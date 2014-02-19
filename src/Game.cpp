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

Game::Game() : running(true), gTest(true), step(0) {
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

	srand(0);

	/*ground = make_shared<HPlane>(Point3(0, -1, 0), 10.0f, 0.0f, 100.0f, COLOR_GRAY);	// reconsider ground Y
	prize = make_shared<Cube>(Point3(0, 0.5f, -48.5), 3.0f, COLOR_AQUA);

	// where do we want to "actually" draw the ground line 0,0,0 ?
	// then change values there cube 0,1,0 and 2.0f makes sense a bit more then 0,0,0, 2.0f

	newBlocks();

	bullet = make_shared<Cube>(Point3(0, 0, 0), 2.0f, COLOR_YELLOW);	// invisible anyway
	bullet->alive = false;*/
	
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
	ground = make_shared<HorizontalPlane>(Point3f(0, -1.1f, 0.0f), 10.0f, 0.1f, 100.0f, COLOR_GRAY);
	bullet = make_shared<Cube>(Point3f(0, 0, 0), COLOR_YELLOW);
	prize = make_shared<Cube>(Point3f(0, 0.0f, 48.5), COLOR_AQUA);

	newBlocks();

	cout << "Entered Main Loop" << endl;

	while (running) {
		gfx->setFrameStart();

		eventSystem->pollEvents();
		handleAllEvents();

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
	if (selected == nullptr) {
		if (eventSystem->isPressed(Key::W)) camera->moveForward();
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
	if (eventSystem->isPressed(Key::RIGHT)) camera->lookRight(20 * 0.05f);

	if (eventSystem->isPressed(Key::SPACE)) onPrimaryAction();
	if (eventSystem->isPressed(Key::ESC)) running = false;
}

void Game::handleMouseEvents() {
	Point2 pos = eventSystem->getMouseDPos();

	camera->lookRight(pos.x * 0.05f);
	camera->lookUp(pos.y * 0.05f);

	if (eventSystem->isPressed(Mouse::BTN_LEFT)) {
		onPrimaryAction();
	}

	if (eventSystem->isPressed(Mouse::BTN_RIGHT)) {
		onSecondaryAction();
	}
}

void Game::onPrimaryAction() {
	bullet->center = camera->getPosition();

	//mainBlocks.front()->transformer.printDebug();
	//camera->printDebug();

	while (selected == nullptr && distanceBetween(camera->getPosition(), bullet->center) < 20.0f) {
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
	ground->move(Vector3f(0, 0, value/2.0f));

	//cout << ground->halfDistZ.getZ() << endl;

	for (auto cube : extraBlocks)
		if (!ground->collidesWith(*cube) && cube->alive && selected != cube)	// kinda gravity for now
			cube->move(gravity);


	buildBlock();
	
	if (isGameWon()) {
		step++;
		mainBlocks.clear();
		extraBlocks.clear();
		newBlocks();
	}
}

// TODO: clean this
void Game::buildBlock() {
	if (selected != nullptr) {
		for (uint i = 0; i < 3; ++i) {
			for (uint j = 0; j < 5; ++j) {
				if (!blocks[j][i]) {
					Point3f c(getValue(j), i*2.0f, 5.0f + 4 * step);
					if (distanceBetween(c, selected->center) < 3.0f) {
						selected->setCenter(c.x, c.y, c.z);
						selected->setLocked(false);
						selected->alive = false;
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
	
	for (auto cube : mainBlocks)
		cube->draw();

	for (auto cube : extraBlocks)
		cube->draw();

	ground->draw();
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

void Game::newBlocks() {
	for (uint i = 0; i < 3; ++i) {
		for (uint j = 0; j < 5; ++j) {
			blocks[j][i] = rand() % 2 == 1;	// set blocks, will need for later
			if (blocks[j][i]) {
				Point3f p(getValue(j), i*2.0f, 5.0f + 4*step);
				mainBlocks.push_back(make_shared<Cube>(p, COLOR_BLUE));
			}
		}
	}

	for (uint i = 0; i < numberOfBlocksRequired(); ++i) {
		Point3f p(getRandom(-4, 4)*1.0f, getRandom(5, 10)*1.0f, -getRandom(25, 35)*1.0f + 5.0f*step);
		extraBlocks.push_back(make_shared<Cube>(p, COLOR_RED));
	}
}
