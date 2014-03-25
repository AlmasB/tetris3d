#include "Game.h"

const float Game::GAME_FPS_DELAY_SEC = (float)GAME_FPS_DELAY_MSEC / __SECOND;

Game::Game() : running(true), currentStep(0), currentCutScene(CutScene::NONE), cutSceneFrame(0), god(false), currentLevel(Level::getNext()) {

	std::vector<std::string> resources;

	// add texture images
	resources.push_back(_RES_TEX_BRICK);
	resources.push_back(_RES_TEX_PRIZE);
	resources.push_back(_RES_TEX_CROSSHAIR);

	// add fonts
	resources.push_back(_RES_FONT);
	
	// add sound files
	resources.push_back(_RES_SFX_CLONG);

	// init engine
	std::shared_ptr<GameEngine> engine;
	try {
		engine = GameEngine::getInstance();
		ResourceManager::loadResources(resources);

		dummy = MD3Object::load("res/upper.md3");	// TODO: remove after tests;
		dummy->setCenter(Point3f(-80, 0, 0));
	}
	catch (EngineException & e) {
		std::cout << "Game::Game()" << e.what() << std::endl;	// note that when EngineException is constructed it prints trace anyway
		throw -1;							// but still nice to have what()
	}

	// engine ready, get subsystems
	gfx = engine->getGraphicsEngine();
	sfx = engine->getAudioEngine();
	eventSystem = engine->getEventEngine();
	physics = engine->getPhysicsEngine();
	camera = gfx->getCamera();

	// adjust settings
	std::string title = "Tetris3D v" + std::to_string(_TETRIS_VERSION_MAJOR) + "." + std::to_string(_TETRIS_VERSION_MINOR) + " by " + _TETRIS_AUTHOR;
	gfx->setWindowTitle(title.c_str());
	gfx->setWindowSize(GAME_W, GAME_H);
	gfx->useFont(ResourceManager::getFont(_RES_FONT));
	eventSystem->setMouseRelative(true);	// trap mouse for FPS
	physics->setGravity(-1.0f, GAME_FPS_DELAY_SEC);

	// init world
	// atm we don't care where we place them, nextLevel() takes care of everything
	prize = std::make_shared<GameObject>(Point3f(0, 0, 0), 2.0f, ResourceManager::getTextureID(_RES_TEX_PRIZE));
	player = std::make_shared<Player>(Point3f(0, 0, 0));
	player->setSensitivity(0.15f);
	crosshair = std::make_shared<GameObject>(Point3f(0, 0, 1), 0.05f, SDL_COLOR_GREEN);
	camera->follow(player);

	dummyCameraObject = std::make_shared<GameObject>(Point3f(0, 0.0f, 0.0f), 2.0f, 0);
	bullet = std::make_shared<GameObject>(Point3f(0, 0, 0), 0.5f, 0);

	initLevel();
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
	debug("The game finished and cleaned up successfully. Press Enter to exit");
	getchar();
#endif
}

int Game::runMainLoop() {
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
	return 0;
}

void Game::update() {
	for (auto block : extraBlocks) {
		if (selected != block) {
			block->applyGravity(*physics);
		}
	}

	// block - platform collision
	for (auto block : extraBlocks) {
		for (auto platform : platforms) {
			if (platform->isColliding(*block)) {
				block->applyAntiGravity(*physics);
				break;
			}
		}
	}

	// block - block collision
	std::list<std::shared_ptr<GameObject>> tmp;
	for (auto obj : extraBlocks)
		tmp.push_back(obj);

	while (!tmp.empty()) {
		std::shared_ptr<GameObject> tmpObj = tmp.front();
		tmp.pop_front();

		for (auto obj : tmp) {
			if (obj->isColliding(*tmpObj)) {
				tmpObj->applyAntiGravity(*physics);
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

	// handle player's grabbed cube
	buildBlock();

	// every five seconds destroy a platform
	if (worldTimer.getElapsed() >= 5 * __SECOND) {
		killPlatform();
		worldTimer.measure();
	}

	if (mainBlocks.size() == currentLevel->width * currentLevel->height) {	// TODO: optimize
		player->addScore(SCORE_PER_STEP);
		nextStep();
	}

	if (player->isColliding(*prize)) {	// player reached prize aka end of level
		player->addScore(SCORE_PER_LEVEL);
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

	prize->draw();

	crosshair->draw();	// fancy tiny spinning cube
	gfx->drawGLTexture(ResourceManager::getTextureID(_RES_TEX_CROSSHAIR), GAME_W / 2 - 15, GAME_H / 2 - 15, 30, 30);	// texture

	gfx->drawText(std::to_string(player->getScore()), SDL_COLOR_GREEN, 50, 50);	// score
	gfx->drawText(std::to_string(gfx->getAverageFPS()) + " FPS", SDL_COLOR_BLUE, GAME_W - 200, 50);	// FPS

	if (dummy)	// MD3 Model
		dummy->draw();
	else
		std::cout << "NO DUMMY" << std::endl;

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

	if (eventSystem->isPressed(Key::UP)) player->lookUp(20);
	if (eventSystem->isPressed(Key::DOWN)) player->lookUp(-20);
	if (eventSystem->isPressed(Key::LEFT)) player->lookRight(-20);
	if (eventSystem->isPressed(Key::RIGHT)) player->lookRight(20);

	if (eventSystem->isPressed(Key::SPACE)) onPrimaryAction();
	if (eventSystem->isPressed(Key::ESC)) running = false;
}

void Game::handlePlayerMovement() {
	float zLine = -currentLevel->length + 16.0f + 6 * currentStep - 1.0f;

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

	while (nullptr == selected && distanceBetween(camera->getCenter(), bullet->getCenter()) < BULLET_DISTANCE) {
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

			player->addScore(SCORE_PER_BLOCK);
			sfx->playSound(ResourceManager::getSound(_RES_SFX_CLONG));
			return;
		}
		++it;
	}
}

void Game::spawnAllBlocks() {
	float z = -currentLevel->length + 16.0f + 6 * currentStep;

	for (int i = 0; i < currentLevel->height; ++i) {
		for (int j = 0; j < currentLevel->width; ++j) {
			float x = 2.0f * j - currentLevel->width;
			float y = i*2.0f + 1.0f;

			if (1 == rand() % 2) {
				mainBlocks.push_back(std::make_shared<GameObject>(Point3f(x, y, z), 2.0f, SDL_COLOR_BLUE));
			}
			else {
				freeBlockSlots.push_back(Point3f(x, y, z));
			}
		}
	}

	int blocksNeeded = currentLevel->width * currentLevel->height - mainBlocks.size();

	for (int i = 0; i < blocksNeeded; ++i) {
		Point3f p((float)getRandom(-currentLevel->width, currentLevel->width), 4 + i*2.0f, player->getCenter().z + getRandom(-5, 5));
		extraBlocks.push_back(std::make_shared<GameObject>(p, 2.0f, getRandomColor(50, 200)));
	}
}

void Game::playCutScene() {
	switch (currentCutScene) {
		case LEVEL_BEGINNING:	playCutSceneLevelBeginning(); break;
		case LEVEL_END:			playCutSceneLevelEnd();		  break;
		case PLAYER_DEATH:		playCutScenePlayerDeath();	  break;
		case GAME_WIN:			playCutSceneGameWin();		  break;
		default:
#ifdef __DEBUG
			debug("Warning:Game::playCutScene", "Unknown CutScene");
#endif
			break;
	}
}

void Game::playCutSceneLevelBeginning() {
	if (cutSceneTimer.getTime() == 0) {	// means running for 1st time
		// move to level beginning -20 units, so player can see how platforms are being built
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
		player->lookAt(prize->getCenter());	// make player look at the prize while falling ;)

		cutSceneTimer.measure();
	}

	if (eventSystem->isPressed(Key::SPACE) || player->getCenter().y <= -30) {
		resetCutScene();
		running = false;
	}
}

void Game::playCutSceneGameWin() {
	player->lookAt(Point3f(0, 0, 0));
	if (cutSceneTimer.getElapsed() >= 0.075 * __SECOND) {
		killPlatform();
		player->addScore(SCORE_PER_BLOCK);
	}

	if (eventSystem->isPressed(Key::SPACE) || platforms.empty()) {
		while (!platforms.empty()) {
			killPlatform();
			player->addScore(SCORE_PER_BLOCK);
		}

		resetCutScene();
		running = false;
	}
}

void Game::resetCutScene() {
	cutSceneFrame = 0;
	cutSceneTimer.reset();
	currentCutScene = CutScene::NONE;
}

void Game::buildPlatforms() {
	std::list<Point2> neighbors;

	for (auto point : openPlatforms) {
		std::list<Point2> tmp = getNeighborPlatforms(point);
		for (auto p : tmp)
			neighbors.push_back(p);

		float x = 2.0f * point.x - currentLevel->width;
		float z = 2.0f * point.y - currentLevel->length;	// place it the way that level center is always at 0.0.0 origin

		// y = -0.1f so that the top of the platforms represent the 0th line in Y - the ground
		std::shared_ptr<GameObject> plat = std::make_shared<GameObject>(Point3f(x, -0.1f, z), 2.0f, 0.2f, 2.0f, ResourceManager::getTextureID(_RES_TEX_BRICK));
		platforms.push_back(plat);
	}

	openPlatforms = neighbors;
}

void Game::killPlatform() {
	if (!platforms.empty()) {
		platforms.pop_front();
	}
}

std::list<Point2> Game::getNeighborPlatforms(Point2 currentNode) {
	std::list<Point2> openNodes;

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

void Game::initLevel() {
	// clear everything associated with level
	mainBlocks.clear();
	extraBlocks.clear();
	freeBlockSlots.clear();
	platforms.clear();
	openPlatforms.clear();

	// tell the "platform builder" where to start
	Point2 firstPlatform(currentLevel->width / 2, 0);
	openPlatforms.push_back(firstPlatform);
	currentLevel->data[firstPlatform.x][firstPlatform.y] = true;

	// return objects to initial state
	player->setCenter(Point3f(-1.0f, 1.0f, -currentLevel->length + 6.0f));	// set player at the beginning + 6 blocks for some margin 
	prize->setCenter(Point3f(-1.0f, 1.0f, currentLevel->length - 2.0f));	// set prize right at the end
	prize->setRotate(0, 0, 0);
	prize->setScale(1, 1, 1);
	currentStep = 0;

	worldTimer.reset();

	currentCutScene = CutScene::LEVEL_BEGINNING;
}

void Game::nextLevel() {
	if (MAX_LEVELS == currentLevel->number) {
		currentCutScene = CutScene::GAME_WIN;
		return;
	}

	currentLevel = Level::getNext();
	initLevel();
}

void Game::nextStep() {
	currentStep++;
	mainBlocks.clear();
	extraBlocks.clear();
	freeBlockSlots.clear();
	spawnAllBlocks();
}
