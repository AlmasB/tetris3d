#include "Game.h"

float getValue(uint n) {
	switch (n) {
		case 0:
			return -4;
		case 1:
			return -2;
		case 2:
			return 0;
		case 3:
			return 2;
		case 4:
			return 4;
	}

	return -20;	// will fix
}

Game::Game() : running(true), currentStep(0), currentCutScene(CutScene::BEGINNING) {
	camera = Camera::getInstance();
	gfx = unique_ptr<GraphicsEngine>(new GraphicsEngine());
	eventSystem = unique_ptr<EventEngine>(new EventEngine());



	cutSceneFrame = 0;
	srand(0);



	Point2 p = { 2, 0 };
	currentNode = p;
	openPlatforms.push_back(currentNode);


	currentLevel = Level::getNext();


	for (uint i = 0; i < 5; ++i) {
		for (uint j = 0; j < 25; ++j) {
			//platformsArray[i][j] = false;
			platformsArray[i][j] = currentLevel->data[i][j];
		}
	}

	platformsArray[2][0] = true;
	
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
		debug("WIN32");	// just a test, no need to use OS specific code yet
	#elif defined(__linux__)
		debug("LINUX");
	#endif
#endif

	std::string error;
	if ((error = gfx->init()) != _ENGINE_ERROR_NONE) {
		std::cout << error << std::endl;
		getchar();
		return false;
	}

	// at this point SDL/GL/GLEW are initialised and OK to use
#ifdef __DEBUG
	debug("GraphicsEngine::init() successful");
#endif

	gfx->setWindowTitle("Tetris3D ~ ");

	


	// where do we want to "actually" draw the ground line 0,0,0 ?
	// then change values there cube 0,1,0 and 2.0f makes sense a bit more then 0,0,0, 2.0f

	bullet = make_shared<Cube>(Point3f(0, 0, 0), 2.0f, COLOR_YELLOW);
	prize = make_shared<Cube>(Point3f(0, 0.0f + 1.0f, currentLevel->length - 2.0f), 2.0f, COLOR_AQUA);
	

	textureBrick = gfx->loadTexture();
	//prize->texture = textureBrick;

	player = make_shared<Player>(Point3f(0, 2.0f, -currentLevel->length + 3*2.0f ));	// give player some back space
	camera->follow(player);

	dummyCameraObject = make_shared<GameObject>(Point3f(0, 0.0f, 0.0f));

	return true;
}

void Game::runMainLoop() {
#ifdef __DEBUG
	debug("Entered Main Loop");
#endif

	while (running) {
		gfx->setFrameStart();

		if (currentCutScene == CutScene::NONE) {
			eventSystem->pollEvents();
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

void Game::handleAllEvents() {
	handleKeyEvents();
	handleMouseEvents();
}

void Game::handleKeyEvents() {
	if (selected == nullptr) {
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
	bullet->setCenter(player->getCenter());

	while (selected == nullptr && distanceBetween(player->getCenter(), bullet->getCenter()) < __BULLET_DISTANCE) {
		bullet->move(camera->getDirection());
		for (auto cube : extraBlocks) {
			if (bullet->collidesWith(*cube)) {
				cube->setLocked(true);
				selected = cube;
				break;
			}
		}
	}
}

void Game::onSecondaryAction() {
	if (selected != nullptr) {
		selected->setLocked(false);
		selected = nullptr;
	}
}

void Game::update() {
	Vector3f gravity(0, -0.01f, 0);

	for (auto block : extraBlocks)
		if (selected != block)
			block->move(gravity);

	for (auto block : extraBlocks) {
		for (auto platform : platforms) {
			if (platform->collidesWith(*block)) {	// TODO: implement box - > plane collision
				block->move(gravity * (-1.0f));
				break;
			}
		}
	}


	buildBlock();
	
	if (isStepCompleted()) {
		nextStep();
	}

	// after updating positions of objects, update camera too
	camera->updateView();
}

// TODO: clean this
void Game::buildBlock() {
	if (selected != nullptr) {
		for (uint i = 0; i < 3; ++i) {
			for (uint j = 0; j < 5; ++j) {
				if (!blocks[j][i]) {
					float x = getValue(j);
					float y = i*2.0f + 1.0f;
					// where player starts count as level beginning
					float z = -currentLevel->length / 2.0f + 3 * 2.0f + 5.0f + 4 * currentStep;

					Point3f c(x, y, z);
					if (distanceBetween(c, selected->getCenter()) < 3.0f) {
						selected->setCenter(c.x, c.y, c.z);
						selected->setLocked(false);
						mainBlocks.push_back(selected);
						extraBlocks.remove(selected);
						blocks[j][i] = true;
						selected = nullptr;
						return;
					}
				}
			}
		}
	}
}

bool Game::isStepCompleted() {
	for (uint i = 0; i < 3; ++i) {
		for (uint j = 0; j < 5; ++j) {
			if (!blocks[j][i]) {
				return false;
			}
		}
	}

	// faster implementation can be checking extraBlocks for 0 size

	return true;
}

void Game::render() {
	gfx->clearScreen();
	
	for (auto cube : mainBlocks)
		cube->draw();

	for (auto cube : extraBlocks)
		cube->draw();

	//ground->draw();
	for (auto plane : platforms)
		plane->draw();

	prize->draw();

	gfx->showScreen();
}

uint Game::numberOfBlocksRequired() {
	uint count = 0;
	for (uint i = 0; i < 3; ++i) {
		for (uint j = 0; j < 5; ++j) {
			if (!blocks[j][i]) {
				count++;
			}
		}
	}

	return count;
}

// TODO: use the length of the platform to determine where to spawn red blocks
void Game::spawnAllBlocks() {
	for (uint i = 0; i < 3; ++i) {
		for (uint j = 0; j < 5; ++j) {
			blocks[j][i] = rand() % 2 == 1;	// set blocks, will need for later
			if (blocks[j][i]) {
				float x = getValue(j);
				float y = i*2.0f + 1.0f;
				// where player starts count as level beginning
				float z = -currentLevel->length / 2.0f + 3 * 2.0f + 5.0f + 4 * currentStep;

				Point3f p(x, y, z);
				mainBlocks.push_back(make_shared<Cube>(p, 2.0f, COLOR_BLUE));
			}
		}
	}

	for (uint i = 0; i < numberOfBlocksRequired(); ++i) {
		Point3f p(getRandom(-currentLevel->width, currentLevel->width)*1.0f, getRandom(10, 15)*1.0f, player->getCenter().z + getRandom(-5, 5));
		extraBlocks.push_back(make_shared<Cube>(p, 2.0f, COLOR_RED));
	}
}

void Game::playCutScene() {
	switch (currentCutScene) {
		case BEGINNING:
			playCutSceneBeginning();
			break;
		case END:
			playCutSceneEnd();
			break;
		case PLAYER_DEATH:
			playCutScenePlayerDeath();
			break;
	}
}

void Game::playCutSceneBeginning() {
	if (cutSceneTimer.getTime() == 0) {	// means running for 1st time
		dummyCameraObject->move(Vector3f(1.0f, 45.0f, 10.0f));	
		camera->follow(dummyCameraObject);
	}

	if (cutSceneTimer.getElapsed() >= 0.075 * __SECOND) {
		buildPlatforms();

		if (dummyCameraObject->getCenter().y > 1.0f) {
			dummyCameraObject->move(Vector3f(0, -1.0f, 0.0f));
		}
		else {
			if (!(dummyCameraObject->getCenter().x >= -5 && dummyCameraObject->getCenter().x <= -2))
				dummyCameraObject->moveRight(0.75f);
			else
				dummyCameraObject->move(Vector3f(0, 0, -1));
		}

		dummyCameraObject->lookAt(prize->getCenter());
		//dummyCameraObject->printDebug(__CENTER);

		cutSceneFrame++;
		cutSceneTimer.measure();
	}

	// TODO: move dummy towards player's center, also platform size is not undetermined, seek boolean coverage
	if (isLevelBuilt() && dummyCameraObject->getCenter().z <= player->getCenter().z) {
		camera->follow(player);
		resetCutScene();
		spawnAllBlocks();
		// TODO: allow platform creation here
		cout << "DEBUG: CutScene::NONE" << endl;
	}
}

void Game::playCutScenePlayerDeath() {

}

void Game::playCutSceneEnd() {

}

void Game::resetCutScene() {
	cutSceneFrame = 0;
	cutSceneTimer.reset();
	currentCutScene = CutScene::NONE;
}

RGBColor Game::getRandomColor() {
	int r = getRandom(0, 4);
	switch (r) {
		case 0: return COLOR_GOLD;
		case 1: return COLOR_RED;
		case 2: return COLOR_GREEN;
		case 3: return COLOR_AQUA;
		case 4: return COLOR_PURPLE;
		default: return COLOR_BLACK;
	}
}

void Game::buildPlatforms() {
	for (auto point : openPlatforms) {
		currentNode = point;
		getNeighborPlatforms();

		float x = getValue(point.x);
		//float y = -1.2f;
		float y = 0.0f;
		float z = 2.0f * point.y - currentLevel->length;	// place it the way that level center is always at 0.0.0 origin


		// y - 0.1f so that the top of the platforms represent the 0th line in Y - the ground
		shared_ptr<Plane> plat = make_shared<Plane>(Point3f(x, y - 0.1f, z), 2.0f, 0.2f, 2.0f, getRandomColor());
		plat->texture = textureBrick;

		platforms.push_back(plat);
	}

	openPlatforms = openPlatforms2;
	openPlatforms2.clear();

	/*cout << "OPEN LIST:" << endl;

	for (auto point : openPlatforms) {
		cout << point.x << " " << point.y << endl;
	}*/
}

void Game::getNeighborPlatforms() {
	int x1 = currentNode.x - 1;
	int x2 = currentNode.x + 1;
	int y1 = currentNode.y - 1;
	int y2 = currentNode.y + 1;

	//cout << currentNode.x << " " << currentNode.y << endl;

	if (x1 >= 0) {
		if (!platformsArray[x1][currentNode.y]) {
			Point2 p = { x1, currentNode.y };
			openPlatforms2.push_back(p);
			platformsArray[x1][currentNode.y] = true;
		}
	}

	if (x2 < 5) {
		if (!platformsArray[x2][currentNode.y]) {
			Point2 p = { x2, currentNode.y };
			openPlatforms2.push_back(p);
			platformsArray[x2][currentNode.y] = true;
		}
	}

	if (y1 >= 0) {
		if (!platformsArray[currentNode.x][y1]) {
			Point2 p = { currentNode.x, y1 };
			openPlatforms2.push_back(p);
			platformsArray[currentNode.x][y1] = true;
		}
	}

	if (y2 < 25) {
		if (!platformsArray[currentNode.x][y2]) {
			Point2 p = { currentNode.x, y2 };
			openPlatforms2.push_back(p);
			platformsArray[currentNode.x][y2] = true;
		}
	}
}

bool Game::isLevelBuilt() {
	for (uint i = 0; i < 5; ++i) {
		for (uint j = 0; j < 25; ++j) {
			if (!platformsArray[i][j])
				return false;
		}
	}

	return true;
}

// TODO: implement
void Game::nextLevel() {

	currentStep = 0; 
}

void Game::nextStep() {
	currentStep++;
	mainBlocks.clear();
	extraBlocks.clear();
	spawnAllBlocks();
}