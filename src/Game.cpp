#include "Game.h"

Game::Game() : running(true), currentStep(0), currentCutScene(CutScene::NONE), cutSceneFrame(0) {
	camera = Camera::getInstance();
	gfx = unique_ptr<GraphicsEngine>(new GraphicsEngine());
	eventSystem = unique_ptr<EventEngine>(new EventEngine());

	srand(0);
	
#ifdef __DEBUG
	debug("Game::Game() finished");
#endif
}

Game::~Game() {
	// TODO: clean after everything is written
}

bool Game::init() {
#ifdef __DEBUG
	#if defined(_WIN32)
		debug("WIN32");
	#elif defined(__linux__)
		debug("LINUX");
	#endif
#endif

	std::string error;
	if (_ENGINE_ERROR_NONE != (error = gfx->init())) {
		std::cout << error << std::endl;
		getchar();
		return false;
	}

	// at this point SDL/GL/GLEW are initialised and OK to use
#ifdef __DEBUG
	debug("GraphicsEngine::init() successful");
#endif

	eventSystem->init();

	gfx->setWindowTitle("Tetris3D ~ ");

	TTF_Font * font = TTF_OpenFont(_RES_FONT, 36);
	gfx->useFont(font);

	vector<string> resources;
	resources.push_back(_RES_TEX_BRICK);
	resources.push_back(_RES_TEX_PRIZE);
	//resources.push_back(_RES_TEX_WALL);
	//resources.push_back(_RES_TEX_DOORUP);
	//resources.push_back(_RES_TEX_DOORDOWN);

	if (_ENGINE_ERROR_NONE != (error = ResourceManager::loadResources(resources))) {
		std::cout << error << std::endl;
		getchar();
		return false;
	}

	// where do we want to "actually" draw the ground line 0,0,0 ?
	// then change values there GameObject 0,1,0 and 2.0f makes sense a bit more then 0,0,0, 2.0f

	// atm we don't care where we place them, nextLevel() takes care of everything
	prize = make_shared<GameObject>(Point3f(0, 0, 0), 2.0f, 2.0f, 2.0f, ResourceManager::getTextureID(_RES_TEX_PRIZE));
	player = make_shared<Player>(Point3f(0, 0, 0));
	camera->follow(player);

	dummyCameraObject = make_shared<GameObject>(Point3f(0, 0.0f, 0.0f), 2.0f, 2.0f, 2.0f, 0);
	bullet = make_shared<GameObject>(Point3f(0, 0, 0), 2.0f, 2.0f, 2.0f, 0);

	scoreboard = make_shared<GameObject>(Point3f(55.0f, 0, 0), 1.0f, 10.0f, 20.0f, 0);

	

	nextLevel();

#ifdef __DEBUG
	debug("Game::init() successful");
#endif

	return true;
}

void Game::runMainLoop() {
#ifdef __DEBUG
	debug("Entered Main Loop");
#endif

	while (running) {	
		gfx->setFrameStart();
		eventSystem->pollEvents();

		if (currentCutScene == CutScene::NONE) {
			handleAllEvents();
		}
		else {
			playCutScene();
		}

		update();
		render();

		gfx->adjustFPSDelay(GAME_FPS_DELAY);
	}

#ifdef __DEBUG
	debug("Exited Main Loop");
#endif
}

void Game::update() {
	Vector3f gravity(0, -0.01f, 0);

	for (auto block : extraBlocks)
		if (selected != block)
			block->move(gravity);

	for (auto block : extraBlocks) {
		for (auto platform : platforms) {
			if (platform->collidesWith(*block)) {	// TODO: implement box - > GameObject collision
				block->move(gravity * (-1.0f));
				break;
			}
		}
	}


	buildBlock();

	if (currentLevel->width * currentLevel->height == mainBlocks.size()) {	// TODO: optimize
		player->addScore(__SCORE_PER_STEP);
		nextStep();
	}

	if (currentCutScene != CutScene::LEVEL_END && player->collidesWith(*prize)) {	// player reached prize aka end of level
		player->addScore(__SCORE_PER_LEVEL);
		currentCutScene = CutScene::LEVEL_END;
	}

	updateUI();

	// after updating positions of objects, update camera last
	camera->updateView();
}

// TODO: something more efficient
void Game::updateUI() {
	//scoreboard->setTexture(gfx->createGLTextureFromText(to_string(player->getScore()), SDL_COLOR_RED));
}

void Game::render() {
	gfx->clearScreen();

	for (auto object : mainBlocks)
		object->draw();

	for (auto object : extraBlocks)
		object->draw();

	for (auto object : platforms)
		object->draw();

	scoreboard->draw();
	prize->draw();

	gfx->showScreen();
}

void Game::handleAllEvents() {
	handleKeyEvents();
	handleMouseEvents();
}

void Game::handleKeyEvents() {
	if (nullptr == selected) {
		if (eventSystem->isPressed(Key::W)) player->moveForward();
		if (eventSystem->isPressed(Key::S)) player->moveBackward();
		if (eventSystem->isPressed(Key::A)) player->moveLeft(0.15f);
		if (eventSystem->isPressed(Key::D)) player->moveRight(0.15f);
	}
	else {
		if (eventSystem->isPressed(Key::W)) selected->move(player->getDirection());
		if (eventSystem->isPressed(Key::S)) selected->move(player->getDirection()*(-1.0f));
		if (eventSystem->isPressed(Key::A)) selected->move(Vector3f(-player->getDirection().z, 0, player->getDirection().x));
		if (eventSystem->isPressed(Key::D)) selected->move(Vector3f(player->getDirection().z, 0, -player->getDirection().x));
	}

	// TODO: values need tweaking for greater experience
	if (eventSystem->isPressed(Key::UP)) player->lookUp(20 * 0.05f);
	if (eventSystem->isPressed(Key::DOWN)) player->lookUp(-20 * 0.05f);
	if (eventSystem->isPressed(Key::LEFT)) player->lookRight(-20 * 0.05f);
	if (eventSystem->isPressed(Key::RIGHT)) player->lookRight(20 * 0.05f);

	if (eventSystem->isPressed(Key::SPACE)) onPrimaryAction();
	if (eventSystem->isPressed(Key::ESC)) running = false;
}

void Game::handleMouseEvents() {
	Point2 pos = eventSystem->getMouseDPos();

	// TODO: values need tweaking for greater experience
	player->lookRight(pos.x * 0.05f);
	player->lookUp(-pos.y * 0.05f);	// invert, because UP is negative in SDL

	if (eventSystem->isPressed(Mouse::BTN_LEFT)) {
		onPrimaryAction();
	}

	if (eventSystem->isPressed(Mouse::BTN_RIGHT)) {
		onSecondaryAction();
	}
}

void Game::onPrimaryAction() {
	if (nullptr != selected)
		return;

	bullet->setCenter(player->getCenter());

	while (nullptr == selected && distanceBetween(player->getCenter(), bullet->getCenter()) < __BULLET_DISTANCE) {
		bullet->move(camera->getDirection());
		for (auto block : extraBlocks) {
			if (bullet->collidesWith(*block)) {
				block->setLocked(true);
				selected = block;
				break;
			}
		}
	}
}

void Game::onSecondaryAction() {
	if (nullptr != selected) {
		selected->setLocked(false);
		selected = nullptr;
	}
}

void Game::buildBlock() {
	if (nullptr == selected)
		return;

	std::list<Point3f>::iterator it = freeBlockSlots.begin();
	while (it != freeBlockSlots.end()) {
		if (distanceBetween(*it, selected->getCenter()) < 3.0f) {
			selected->setCenter((*it).x, (*it).y, (*it).z);
			selected->setLocked(false);
			mainBlocks.push_back(selected);
			extraBlocks.remove(selected);
			it = freeBlockSlots.erase(it);
			selected = nullptr;

			player->addScore(__SCORE_PER_BLOCK);
			scoreboard->setTexture(gfx->createGLTextureFromText(to_string(player->getScore()), SDL_COLOR_RED));
			return;
		}
		++it;
	}
}

void Game::spawnAllBlocks() {
	// where player starts count as level start
	float z = -currentLevel->length / 2.0f + 3 * 2.0f + 4.0f + 4 * currentStep + 1.0f;

	for (int i = 0; i < currentLevel->height; ++i) {
		for (int j = 0; j < currentLevel->width; ++j) {
			float x = 2.0f * j - currentLevel->width;
			float y = i*2.0f + 1.0f;

			if (1 == rand() % 2) {
				mainBlocks.push_back(make_shared<GameObject>(Point3f(x, y, z), 2.0f, 2.0f, 2.0f, SDL_COLOR_BLUE));
			}
			else {
				freeBlockSlots.push_back(Point3f(x, y, z));
			}
		}
	}

	int blocksNeeded = currentLevel->width * currentLevel->height - mainBlocks.size();

	for (int i = 0; i < blocksNeeded; ++i) {
		Point3f p(getRandom(-currentLevel->width, currentLevel->width)*1.0f, getRandom(10, 15)*1.0f, player->getCenter().z + getRandom(-5, 5));
		extraBlocks.push_back(make_shared<GameObject>(p, 2.0f, 2.0f, 2.0f, SDL_COLOR_RED));
	}
}

void Game::playCutScene() {
	switch (currentCutScene) {
		case LEVEL_BEGINNING:
			playCutSceneLevelBeginning();
			break;
		case LEVEL_END:
			playCutSceneLevelEnd();
			break;
		case PLAYER_DEATH:
			playCutScenePlayerDeath();
			break;
	}
}

void Game::playCutSceneLevelBeginning() {
	if (cutSceneTimer.getTime() == 0) {	// means running for 1st time
		dummyCameraObject->moveTo(Point3f(1.0f, 45.0f, -currentLevel->length*1.0f - 20.0f));
		dummyCameraObject->lookAt(prize->getCenter());
		camera->follow(dummyCameraObject);
	}

	if (cutSceneTimer.getElapsed() >= 0.075 * __SECOND) {
		buildPlatforms();

		if (dummyCameraObject->getCenter().y > 7.0f) {
			dummyCameraObject->move(Vector3f(0, -1.0f, 0.25f));
		}
		else {
			if (distanceBetween(dummyCameraObject->getCenter(), prize->getCenter()) > 7.5f) {
				dummyCameraObject->move(Vector3f(0, 0, 1.5f));
			}
			else {
				cutSceneFrame++;
			}
		}

		dummyCameraObject->lookAt(prize->getCenter());

		cutSceneTimer.measure();
	}
	
	if (eventSystem->isPressed(Key::SPACE) || (isLevelBuilt() && cutSceneFrame > 0)) {
		camera->follow(player);
		resetCutScene();
		spawnAllBlocks();
#ifdef __DEBUG
		debug("Level Created, Blocks spawned, CutScene::LEVEL_BEGINNING completed");
#endif
	}
}

void Game::playCutSceneLevelEnd() {
	if (cutSceneTimer.getTime() == 0) {	// means running for 1st time
		dummyCameraObject->moveTo(player->getCenter());
		dummyCameraObject->move(Vector3f(0, 0, -5.0f));	// move slightly back
		dummyCameraObject->lookAt(prize->getCenter());
		camera->follow(dummyCameraObject);
	}

	if (cutSceneTimer.getElapsed() >= 0.025 * __SECOND) {
		if (prize->getCenter().y < 3.0f) {
			prize->move(Vector3f(0, 0.1f, 0));
			dummyCameraObject->lookAt(prize->getCenter());
		}
		else {
			prize->rotate(0, 2.0f, 0);
			cutSceneFrame++;
		}

		if (cutSceneFrame >= 125) {
			prize->scale(-0.025f, -0.025f, -0.025f);
		}

		cutSceneTimer.measure();
	}

	if (eventSystem->isPressed(Key::SPACE) || prize->getScale().x <= 0) {
		camera->follow(player);
		resetCutScene();
		nextLevel();
	}
}

void Game::playCutScenePlayerDeath() {

}

void Game::playCutSceneGameWin() {

}

void Game::playCutSceneGameLose() {

}

void Game::resetCutScene() {
	cutSceneFrame = 0;
	cutSceneTimer.reset();
	currentCutScene = CutScene::NONE;
}

void Game::buildPlatforms() {
	for (auto point : openPlatforms) {
		currentNode = point;
		getNeighborPlatforms();

		float x = 2.0f * point.x - currentLevel->width;
		float y = 0.0f;
		float z = 2.0f * point.y - currentLevel->length;	// place it the way that level center is always at 0.0.0 origin


		// y - 0.1f so that the top of the platforms represent the 0th line in Y - the ground
		shared_ptr<GameObject> plat = make_shared<GameObject>(Point3f(x, y - 0.1f, z), 2.0f, 0.2f, 2.0f, ResourceManager::getTextureID(_RES_TEX_BRICK));
		platforms.push_back(plat);
	}

	openPlatforms = openPlatforms2;
	openPlatforms2.clear();
}

void Game::getNeighborPlatforms() {
	int x1 = currentNode.x - 1;
	int x2 = currentNode.x + 1;
	int y1 = currentNode.y - 1;
	int y2 = currentNode.y + 1;

	if (x1 >= 0) {
		if (!currentLevel->data[x1][currentNode.y]) {
			Point2 p = { x1, currentNode.y };
			openPlatforms2.push_back(p);
			currentLevel->data[x1][currentNode.y] = true;
		}
	}

	if (x2 < currentLevel->width) {
		if (!currentLevel->data[x2][currentNode.y]) {
			Point2 p = { x2, currentNode.y };
			openPlatforms2.push_back(p);
			currentLevel->data[x2][currentNode.y] = true;
		}
	}

	if (y1 >= 0) {
		if (!currentLevel->data[currentNode.x][y1]) {
			Point2 p = { currentNode.x, y1 };
			openPlatforms2.push_back(p);
			currentLevel->data[currentNode.x][y1] = true;
		}
	}

	if (y2 < currentLevel->length) {
		if (!currentLevel->data[currentNode.x][y2]) {
			Point2 p = { currentNode.x, y2 };
			openPlatforms2.push_back(p);
			currentLevel->data[currentNode.x][y2] = true;
		}
	}
}

bool Game::isLevelBuilt() {
	for (int i = 0; i < currentLevel->width; ++i) {
		for (int j = 0; j < currentLevel->length; ++j) {
			if (!currentLevel->data[i][j])
				return false;
		}
	}

	return true;
}

void Game::nextLevel() {
	if (Level::getNumberOfLevels() == __MAX_LEVELS) {
		currentCutScene = CutScene::GAME_WIN;
		return;
	}


	mainBlocks.clear();
	extraBlocks.clear();
	freeBlockSlots.clear();
	platforms.clear();
	openPlatforms.clear();
	openPlatforms2.clear();

	// TODO: clean that
	Point2 p = { 2, 0 };
	currentNode = p;
	openPlatforms.push_back(currentNode);

	currentLevel = Level::getNext();

	currentLevel->data[2][0] = true;



	player->setCenter(Point3f(0-1.0f, 2.0f, -currentLevel->length + 3 * 2.0f));
	prize->setCenter(Point3f(0-1.0f, 0.0f + 1.0f, currentLevel->length - 2.0f));
	prize->setRotate(0, 0, 0);
	prize->setScale(1, 1, 1);
	currentStep = 0; 
	
	
	currentCutScene = CutScene::LEVEL_BEGINNING;
}

void Game::nextStep() {
	currentStep++;
	mainBlocks.clear();
	extraBlocks.clear();
	freeBlockSlots.clear();
	spawnAllBlocks();
}
