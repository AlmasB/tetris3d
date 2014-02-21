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

Game::Game() : running(true), step(0), currentCutScene(CutScene::BEGINNING) {
	camera = Camera::getInstance();
	gfx = unique_ptr<GraphicsEngine>(new GraphicsEngine());
	eventSystem = unique_ptr<EventEngine>(new EventEngine());

#if defined(_WIN64)
	cout << "WIN64" << endl;
#elif defined(_WIN32)
	cout << "WIN32" << endl;
#elif defined(__linux__)
	cout << "LINUX" << endl;
#endif

	cutSceneFrame = 0;
	srand(0);

	Point2 p = { 0, 0 };
	currentNode = p;

	Point2 p1 = { 4, 0 };

	openPlatforms.push_back(currentNode);
	openPlatforms.push_back(p1);

	for (uint i = 0; i < 5; ++i) {
		for (uint j = 0; j < 25; ++j) {
			platformsArray[i][j] = false;
		}
	}

	platformsArray[0][0] = true;
	platformsArray[4][0] = true;

	test = false;

	/*ground = make_shared<HPlane>(Point3(0, -1, 0), 10.0f, 0.0f, 100.0f, COLOR_GRAY);	// reconsider ground Y
	prize = make_shared<Cube>(Point3(0, 0.5f, -48.5), 3.0f, COLOR_AQUA);

	// where do we want to "actually" draw the ground line 0,0,0 ?
	// then change values there cube 0,1,0 and 2.0f makes sense a bit more then 0,0,0, 2.0f
	*/
	
	cout << "Game::Game() finished" << endl;
}

Game::~Game() {

	// TODO: clean after everything is written
	// atm only smart pointers, they clean up after themselves dont they?
}

bool Game::init() {
	std::string error;
	if ((error = gfx->init()) != _ENGINE_ERROR_NONE) {
		std::cout << error << std::endl;
		getchar();
		return false;
	}

	// do the rest of game init here, at this point GL SDL GLEW are OK
	gfx->setWindowTitle("Tetris3D ~ ");

	return true;
}

void Game::runMainLoop() {
	// move to init or smth
	float length = 50.0f;
	ground = make_shared<Plane>(Point3f(0, -1.9f, 0.0f), 10.0f, 0.1f, length, COLOR_GRAY);
	bullet = make_shared<Cube>(Point3f(0, 0, 0), 2.0f, COLOR_YELLOW);
	prize = make_shared<Cube>(Point3f(0, 0.0f, length / 2.0f -1.0f), 2.0f, COLOR_AQUA);

	player = make_shared<Player>(Point3f(0, 0, -15.0f));
	camera->follow(player);

	dummy = make_shared<GameObject>(Point3f(0, 65.0f, 10.0f));

	newBlocks();

	cout << "Entered Main Loop" << endl;

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
}

void Game::handleAllEvents() {
	handleKeyEvents();
	handleMouseEvents();
}

void Game::handleKeyEvents() {
	/*if (selected == nullptr) {
		if (eventSystem->isPressed(Key::W)) camera->moveForward();	// move player and assign camera to player eyes
		if (eventSystem->isPressed(Key::S)) camera->moveBackward();
		if (eventSystem->isPressed(Key::A)) camera->moveLeft();
		if (eventSystem->isPressed(Key::D)) camera->moveRight();
	}
	else {
		if (eventSystem->isPressed(Key::W)) selected->move(camera->getDirection());
		if (eventSystem->isPressed(Key::S)) selected->move(camera->getDirection()*(-1.0f));
		if (eventSystem->isPressed(Key::A)) selected->move(Vector3f(-camera->getDirection().z, 0, camera->getDirection().x));
		if (eventSystem->isPressed(Key::D)) selected->move(Vector3f(camera->getDirection().z, 0, -camera->getDirection().x));
	}

	// values need tweaking for greater experience
	if (eventSystem->isPressed(Key::UP)) camera->lookUp(-20 * 0.05f);
	if (eventSystem->isPressed(Key::DOWN)) camera->lookUp(20 * 0.05f);
	if (eventSystem->isPressed(Key::LEFT)) camera->lookRight(-20 * 0.05f);
	if (eventSystem->isPressed(Key::RIGHT)) camera->lookRight(20 * 0.05f);*/

	if (selected == nullptr) {
		if (eventSystem->isPressed(Key::W)) player->moveForward();	// move player and assign camera to player eyes
		if (eventSystem->isPressed(Key::S)) player->moveBackward();
		if (eventSystem->isPressed(Key::A)) player->moveLeft();
		if (eventSystem->isPressed(Key::D)) player->moveRight();
	}
	else {
		if (eventSystem->isPressed(Key::W)) selected->move(player->getDirection());
		if (eventSystem->isPressed(Key::S)) selected->move(player->getDirection()*(-1.0f));
		if (eventSystem->isPressed(Key::A)) selected->move(Vector3f(-player->getDirection().z, 0, player->getDirection().x));
		if (eventSystem->isPressed(Key::D)) selected->move(Vector3f(player->getDirection().z, 0, -player->getDirection().x));
	}

	// values need tweaking for greater experience
	if (eventSystem->isPressed(Key::UP)) player->lookUp(-20 * 0.05f);
	if (eventSystem->isPressed(Key::DOWN)) player->lookUp(20 * 0.05f);
	if (eventSystem->isPressed(Key::LEFT)) player->lookRight(-20 * 0.05f);
	if (eventSystem->isPressed(Key::RIGHT)) player->lookRight(20 * 0.05f);

	if (eventSystem->isPressed(Key::SPACE)) onPrimaryAction();
	if (eventSystem->isPressed(Key::ESC)) running = false;
}

void Game::handleMouseEvents() {
	Point2 pos = eventSystem->getMouseDPos();

	//camera->lookRight(pos.x * 0.05f);
	//camera->lookUp(pos.y * 0.05f);
	player->lookRight(pos.x * 0.05f);
	player->lookUp(pos.y * 0.05f);

	//cout << "lookRight(" << pos.x*0.05f << ")" << endl;

	if (eventSystem->isPressed(Mouse::BTN_LEFT)) {
		onPrimaryAction();
	}

	if (eventSystem->isPressed(Mouse::BTN_RIGHT)) {
		onSecondaryAction();
	}
}

void Game::onPrimaryAction() {
	bullet->setCenter(player->getCenter());

	cout << camera->getDirection().x << " " << camera->getDirection().y << " " << camera->getDirection().z << endl;

	//mainBlocks.front()->transformer.printDebug();
	//camera->printDebug();

	while (selected == nullptr && distanceBetween(player->getCenter(), bullet->getCenter()) < 20.0f) {
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

	float value = 0.0025f;

	//ground->setDistZ(ground->halfDistZ.z * 2 - value);	// we could change VBO here
	//ground->move(Vector3f(0, 0, value/2.0f));
	//prize->move(Vector3f(0, 0, value / 2.0f));

	//cout << ground->halfDistZ.getZ() << endl;

	for (auto cube : extraBlocks)
		if (!ground->collidesWith(*cube) && selected != cube)
			cube->move(gravity);


	buildBlock();
	
	if (isGameWon()) {
		step++;
		mainBlocks.clear();
		extraBlocks.clear();
		newBlocks();
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
					Point3f c(getValue(j), i*2.0f, 5.0f + 4 * step);
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

bool Game::isGameWon() {
	for (uint i = 0; i < 3; ++i) {
		for (uint j = 0; j < 5; ++j) {
			if (!blocks[j][i]) {
				return false;
			}
		}
	}

	return true;
}

void Game::render() {
	gfx->clearScreen();
	
	/*for (auto cube : mainBlocks)
		cube->draw();

	for (auto cube : extraBlocks)
		cube->draw();*/

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
void Game::newBlocks() {
	for (uint i = 0; i < 3; ++i) {
		for (uint j = 0; j < 5; ++j) {
			blocks[j][i] = rand() % 2 == 1;	// set blocks, will need for later
			if (blocks[j][i]) {
				Point3f p(getValue(j), i*2.0f, 5.0f + 4*step);
				mainBlocks.push_back(make_shared<Cube>(p, 2.0f, COLOR_BLUE));
			}
		}
	}

	for (uint i = 0; i < numberOfBlocksRequired(); ++i) {
		Point3f p(getRandom(-4, 4)*1.0f, getRandom(5, 10)*1.0f, -getRandom(25, 35)*1.0f + 5.0f*step);
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
		//shared_ptr<Movable> dummy = make_shared<GameObject>(Point3f(0, 65.0f, 10.0f));
		//dummy->move(Vector3f(0, 15.0f, 0));
		dummy->lookUp(90.0f);
		
		camera->follow(dummy);

		test2 = 0;
	}


	if (cutSceneTimer.getElapsed() >= __SECOND * 0.075) {
		buildPlatforms();

		if (dummy->getCenter().y > 10.0f) {
			dummy->move(Vector3f(0, -1.0f, 0.1f));
			//cout << dummy->getCenter().y << endl;
		}
		else {
			if (test2 == 0) {
				dummy->lookUp(-90.0f);
				dummy->move(Vector3f(0, -10.0f, 0.0f));
			}


			test2++;
			dummy->moveRight();
			//dummy->lookRight(-1.7f);

			/*float x0 = dummy->getCenter().x;
			float z0 = dummy->getCenter().z;

			float x1 = prize->getCenter().x;
			float z1 = prize->getCenter().z;

			float dx = x1 - x0;
			float dz = z1 - z0;

			float theta = toDegree(asin(dx / sqrtf(dx*dx + dz*dz)));

			dummy->lookRight(theta);*/

			dummy->direction = prize->getCenter() - dummy->getCenter();

			if (test2 > 70)
				test = true;
			//test = true;
		}

		cutSceneTimer.measure();
	}

	if (platforms.size() >= 125 && test) {
		cutSceneFrame = 0;
		cutSceneTimer.reset();
		currentCutScene = CutScene::NONE;

		camera->follow(player);

		cout << "DEBUG: CutScene::NONE" << endl;
		//cout << isFull() << endl;
	}
}

void Game::playCutScenePlayerDeath() {

}

void Game::playCutSceneEnd() {

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
		float y = -1.2f;
		float z = 2.0f * point.y - 12;

		platforms.push_back(make_shared<Plane>(Point3f(x, y, z), 2.0f, 0.2f, 2.0f, getRandomColor()));
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

bool Game::isFull() {
	for (uint i = 0; i < 5; ++i) {
		for (uint j = 0; j < 25; ++j) {
			if (!platformsArray[i][j])
				return false;
		}
	}

	return true;
}
