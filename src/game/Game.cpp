#include "Game.h"

Game::Game() : running(true), currentStep(0), currentCutScene(CutScene::NONE), cutSceneFrame(0), god(true) {
}

Game::~Game() {
#ifdef __DEBUG
	debug("Game::~Game() started");
#endif

	// kill Game class' instance pointers
	// so that engine is isolated from the outside world
	// before shutting down
	camera.reset();
	gfx.reset();
	eventSystem.reset();

	// kill engine
	GameEngine::quit();

#ifdef __DEBUG
	debug("Game::~Game() finished");
#endif
}

bool Game::init() {
	vector<string> resources;
	resources.push_back(_RES_TEX_BRICK);
	resources.push_back(_RES_TEX_PRIZE);
	resources.push_back("res/crosshair.png");
	resources.push_back(_RES_FONT);

	resources.push_back("res/audio/clong.wav");

	//resources.push_back(_RES_TEX_WALL);
	//resources.push_back(_RES_TEX_DOORUP);
	//resources.push_back(_RES_TEX_DOORDOWN);

	shared_ptr<GameEngine> engine = GameEngine::getInstance();
	if (!engine->init(resources))
		return false;

	gfx = engine->getGraphicsEngine();
	sfx = engine->getAudioEngine();
	eventSystem = engine->getEventEngine();
	camera = gfx->getCamera();

	gfx->setWindowTitle("Tetris3D ~ ");
	gfx->useFont(ResourceManager::getFont(_RES_FONT));

	// where do we want to "actually" draw the ground line 0,0,0 ?
	// then change values there GameObject 0,1,0 and 2.0f makes sense a bit more then 0,0,0, 2.0f

	// atm we don't care where we place them, nextLevel() takes care of everything
	prize = make_shared<GameObject>(Point3f(0, 0, 0), 2.0f, 2.0f, 2.0f, ResourceManager::getTextureID(_RES_TEX_PRIZE));
	player = make_shared<Player>(Point3f(0, 0, 0));
	player->setSensitivity(0.15f);
	crosshair = make_shared<GameObject>(Point3f(0, 0, 1), 0.05f, 0.05f, 0.05f, SDL_COLOR_GREEN);
	camera->follow(player);

	dummyCameraObject = make_shared<GameObject>(Point3f(0, 0.0f, 0.0f), 2.0f, 2.0f, 2.0f, 0);
	bullet = make_shared<GameObject>(Point3f(0, 0, 0), 0.5f, 0.5f, 0.5f, 0);
	testObj = make_shared<GameObject>(Point3f(1.0f, 1.0f, -25.0f), 2.0f, 2.0f, 2.0f, SDL_COLOR_RED);

	scoreboard = make_shared<GameObject>(Point3f(55.0f, 0, 0), 1.0f, 10.0f, 20.0f, 0);
	//scoreboard->setTexture(gfx->createGLTextureFromText(to_string(player->getScore()), SDL_COLOR_YELLOW));

	srand(SDL_GetTicks());	// should be on engine side

	cross = IMG_Load("res/crosshair.png");

	dummy = new MD3Object("res/upper.md3");

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
			update();
		}
		else {
			if (eventSystem->isPressed(Key::ESC))	// debug escape
				running = false;
			playCutScene();
		}

		camera->updateView();
		render();

		gfx->adjustFPSDelay(GAME_FPS_DELAY_MSEC);
	}

#ifdef __DEBUG
	debug("Exited Main Loop");
#endif
}

void Game::update() {
	// gravity value 1.0f, set gravity and update time to phys engine
	Vector3f gravity(0, -GAME_FPS_DELAY_SEC * 1.0f, 0);

	for (auto block : extraBlocks) {
		if (selected != block) {
			block->move(gravity);
		}
	}

	// block - platform collision
	for (auto block : extraBlocks) {
		for (auto platform : platforms) {
			if (platform->isColliding(*block)) {
				block->move(gravity * -1.0f);
				break;
			}
		}
	}

	// block - block collision
	list<shared_ptr<GameObject>> tmp;
	for (auto obj : extraBlocks)
		tmp.push_back(obj);

	while (!tmp.empty()) {
		shared_ptr<GameObject> tmpObj = tmp.front();
		tmp.pop_front();

		for (auto obj : tmp) {
			if (obj->isColliding(*tmpObj)) {
				tmpObj->move(gravity * -1.0f);
				break;
			}
		}
	}

	// player - platform collision
	if (!god) {
		bool death = true;

		for (auto plat : platforms) {
			if (player->isColliding(*plat)) {
				death = false;
				break;
			}
		}

		if (death)
			currentCutScene = CutScene::PLAYER_DEATH;
	}

	buildBlock();

	if (worldTimer.getElapsed() >= 5 * __SECOND) {
		killPlatforms();
		worldTimer.measure();
	}

	if (mainBlocks.size() == currentLevel->width * currentLevel->height) {	// TODO: optimize
		player->addScore(__SCORE_PER_STEP);
		nextStep();
	}

	if (player->isColliding(*prize)) {	// player reached prize aka end of level
		player->addScore(__SCORE_PER_LEVEL);
		currentCutScene = CutScene::LEVEL_END;
	}

	crosshair->setCenter(player->getCenter() + Vector3f(0, 1.0f, 0) + player->getDirection() * 2.0f);
	crosshair->setRotate(player->getVerAngle(), -player->getHorAngle(), -player->getVerAngle());
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
	crosshair->draw();

	gfx->drawText(to_string(player->getScore()), SDL_COLOR_GREEN, 50, 50);
	gfx->drawText(to_string(gfx->getAverageFPS()) + " FPS", SDL_COLOR_BLUE, 600, 50);

	//gfx->drawText(to_string(player->getLives()), SDL_COLOR_GREEN, 50, 50);
	gfx->drawSDLSurface(cross, 800/2 - 15, 600/2 - 15, 30, 30);

	if (dummy)
		dummy->draw();
	else
		cout << "NO DUMMY" << endl;

	gfx->showScreen();
}

void Game::handleAllEvents() {
	handleKeyEvents();
	handleMouseEvents();
}

void Game::handleKeyEvents() {
	if (nullptr == selected) {
		handlePlayerMovement();
	}
	else {
		if (eventSystem->isPressed(Key::W)) selected->move(player->getDirection());
		if (eventSystem->isPressed(Key::S)) selected->move(player->getDirection()*(-1.0f));
		if (eventSystem->isPressed(Key::A)) selected->move(Vector3f(-player->getDirection().z, 0, player->getDirection().x));
		if (eventSystem->isPressed(Key::D)) selected->move(Vector3f(player->getDirection().z, 0, -player->getDirection().x));
	}

	// TODO: values need tweaking for greater experience
	if (eventSystem->isPressed(Key::UP)) player->lookUp(20);
	if (eventSystem->isPressed(Key::DOWN)) player->lookUp(-20);
	if (eventSystem->isPressed(Key::LEFT)) player->lookRight(-20);
	if (eventSystem->isPressed(Key::RIGHT)) player->lookRight(20);

	if (eventSystem->isPressed(Key::SPACE)) onPrimaryAction();
	if (eventSystem->isPressed(Key::ESC)) running = false;
}

void Game::handlePlayerMovement() {
	float zLine = -currentLevel->length / 2.0f + 3 * 2.0f + 4.0f + 4 * currentStep - 1.0f;

	if (eventSystem->isPressed(Key::W)) {
		player->moveForward();
		if (player->getCenter().z > zLine && !god)
			player->moveBackward();
	}

	if (eventSystem->isPressed(Key::S)) {
		player->moveBackward();
		if (player->getCenter().z > zLine && !god)
			player->moveForward();
	}

	if (eventSystem->isPressed(Key::A)) {
		player->moveLeft(0.15f);
		if (player->getCenter().z > zLine && !god)
			player->moveRight(0.15f);
	}

	if (eventSystem->isPressed(Key::D)) {
		player->moveRight(0.15f);
		if (player->getCenter().z > zLine && !god)
			player->moveLeft(0.15f);
	}
}

void Game::handleMouseEvents() {
	Point2 pos = eventSystem->getMouseDPos();

	player->lookRight((float)pos.x);
	player->lookUp((float)-pos.y);	// invert, because UP is negative in SDL

	if (eventSystem->isPressed(Mouse::BTN_LEFT)) onPrimaryAction();
	if (eventSystem->isPressed(Mouse::BTN_RIGHT)) onSecondaryAction();
}

void Game::onPrimaryAction() {
	if (selected != nullptr)
		return;

	bullet->setCenter(camera->getCenter());

	while (nullptr == selected && distanceBetween(camera->getCenter(), bullet->getCenter()) < __BULLET_DISTANCE) {
		bullet->move(camera->getDirection());
		for (auto block : extraBlocks) {
			if (bullet->isColliding(*block)) {
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
			sfx->playSound(ResourceManager::getSound("res/audio/clong.wav"));
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
		Point3f p(getRandom(-currentLevel->width, currentLevel->width)*1.0f, 4 + i*2.0f, player->getCenter().z + getRandom(-5, 5));
		//Point3f p(i*2.0f, 4 + i*4.0f, -25.0f);
		extraBlocks.push_back(make_shared<GameObject>(p, 2.0f, 2.0f, 2.0f, getRandomColor(50, 200)));
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
		while (!isLevelBuilt()) {
			buildPlatforms();
		}

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
	if (cutSceneTimer.getElapsed() >= 0.025 * __SECOND) {
		player->move(Vector3f(0, -0.25, 0));
		player->lookAt(prize->getCenter());

		cutSceneTimer.measure();
	}

	if (eventSystem->isPressed(Key::SPACE) || player->getCenter().y <= -30) {
		resetCutScene();
		running = false;
	}
}

void Game::playCutSceneGameWin() {
	// TODO: do something fancy here as player won the game
}

void Game::resetCutScene() {
	cutSceneFrame = 0;
	cutSceneTimer.reset();
	currentCutScene = CutScene::NONE;
}

void Game::buildPlatforms() {
	list<Point2> neighbors;

	for (auto point : openPlatforms) {
		list<Point2> tmp = getNeighborPlatforms(point);
		for (auto p : tmp)
			neighbors.push_back(p);

		float x = 2.0f * point.x - currentLevel->width;
		float y = 0.0f;
		float z = 2.0f * point.y - currentLevel->length;	// place it the way that level center is always at 0.0.0 origin


		// y - 0.1f so that the top of the platforms represent the 0th line in Y - the ground
		shared_ptr<GameObject> plat = make_shared<GameObject>(Point3f(x, y - 0.1f, z), 2.0f, 0.2f, 2.0f, ResourceManager::getTextureID(_RES_TEX_BRICK));
		platforms.push_back(plat);
	}

	openPlatforms = neighbors;
}

void Game::killPlatforms() {
	if (!platforms.empty()) {
		platforms.pop_front();
	}
}

list<Point2> Game::getNeighborPlatforms(Point2 currentNode) {
	list<Point2> openNodes;

	int x1 = currentNode.x - 1;
	int x2 = currentNode.x + 1;
	int y1 = currentNode.y - 1;
	int y2 = currentNode.y + 1;

	if (x1 >= 0) {
		if (!currentLevel->data[x1][currentNode.y]) {
			openNodes.push_back(Point2(x1, currentNode.y));
			currentLevel->data[x1][currentNode.y] = true;
		}
	}

	if (x2 < currentLevel->width) {
		if (!currentLevel->data[x2][currentNode.y]) {
			openNodes.push_back(Point2(x2, currentNode.y));
			currentLevel->data[x2][currentNode.y] = true;
		}
	}

	if (y1 >= 0) {
		if (!currentLevel->data[currentNode.x][y1]) {
			openNodes.push_back(Point2(currentNode.x, y1));
			currentLevel->data[currentNode.x][y1] = true;
		}
	}

	if (y2 < currentLevel->length) {
		if (!currentLevel->data[currentNode.x][y2]) {
			openNodes.push_back(Point2(currentNode.x, y2));
			currentLevel->data[currentNode.x][y2] = true;
		}
	}

	return openNodes;
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

void Game::resetLevel() {
	// clear everything associated with level
	mainBlocks.clear();
	extraBlocks.clear();
	freeBlockSlots.clear();
	platforms.clear();
	openPlatforms.clear();

	openPlatforms.push_back(Point2(currentLevel->width / 2, 0));
	currentLevel->data[currentLevel->width / 2][0] = true;

	// return objects to initial state
	player->setCenter(Point3f(0 - 1.0f, 1.0f, -currentLevel->length + 3 * 2.0f));
	prize->setCenter(Point3f(0 - 1.0f, 0.0f + 1.0f, currentLevel->length - 2.0f));
	prize->setRotate(0, 0, 0);
	prize->setScale(1, 1, 1);
	currentStep = 0;

	worldTimer.reset();

	currentCutScene = CutScene::LEVEL_BEGINNING;
}

void Game::nextLevel() {
	if (Level::getNumberOfLevels() == __MAX_LEVELS) {
		currentCutScene = CutScene::GAME_WIN;
		return;
	}

	// init the level
	currentLevel = Level::getNext();
	resetLevel();
}

void Game::nextStep() {
	currentStep++;
	mainBlocks.clear();
	extraBlocks.clear();
	freeBlockSlots.clear();
	spawnAllBlocks();
}
