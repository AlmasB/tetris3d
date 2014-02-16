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
	camera = make_unique<Camera>();
	gfx =  make_unique<GraphicsEngine>();
	eventSystem = make_unique<EventEngine>();

	// some platform related experiments

#if defined(_WIN64)
	cout << "WIN64" << endl;
#elif defined(_WIN32)
	cout << "WIN32" << endl;
#elif defined(__linux__)
	cout << "LINUX" << endl;
#endif

	srand(0);

	ground = make_shared<HPlane>(Point3(0, -1, 0), 10.0f, 0.0f, 100.0f, COLOR_GRAY);	// reconsider ground Y
	prize = make_shared<Cube>(Point3(0, 0.5f, -48.5), 3.0f, COLOR_AQUA);

	// where do we want to "actually" draw the ground line 0,0,0 ?
	// then change values there cube 0,1,0 and 2.0f makes sense a bit more then 0,0,0, 2.0f

	newBlocks();

	bullet = make_shared<Cube>(Point3(0, 0, 0), 2.0f, COLOR_YELLOW);	// invisible anyway
	bullet->alive = false;
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

	return true;
}

void Game::runMainLoop() {
	Uint32 start, end;

	while (running) {
		start = SDL_GetTicks();
		eventSystem->pollEvents();

		handleKeyEvents();
		handleMouseEvents();

		update();
		render();

		end = SDL_GetTicks() - start;
		if (end < 20) {
			SDL_Delay(20 - end);
		}
	}
}

void Game::handleKeyEvents() {
	if (selected == NULL) {
		if (eventSystem->isPressed(Key::W))
			camera->moveForward();
		if (eventSystem->isPressed(Key::S))
			camera->moveBack();
		if (eventSystem->isPressed(Key::A))
			camera->moveLeft();
		if (eventSystem->isPressed(Key::D))
			camera->moveRight();
	}
	else {
		if (eventSystem->isPressed(Key::W))
			selected->move(camera->getDirection());
		if (eventSystem->isPressed(Key::S))
			selected->move(camera->getDirection()*(-1.0f));
		if (eventSystem->isPressed(Key::A))
			selected->move(Vector3(camera->getDirection().getZ(), 0, -camera->getDirection().getX()));
		if (eventSystem->isPressed(Key::D))
			selected->move(Vector3(-camera->getDirection().getZ(), 0, camera->getDirection().getX()));
	}

	if (eventSystem->isPressed(Key::LEFT))
		camera->lookRight(-1.0f * 0.035f);
	if (eventSystem->isPressed(Key::RIGHT))
		camera->lookRight(1.0f * 0.035f);
	if (eventSystem->isPressed(Key::UP))
		camera->lookUp(1.0f * 0.035f);
	if (eventSystem->isPressed(Key::DOWN))
		camera->lookUp(-1.0f * 0.035f);

	if (eventSystem->isPressed(Key::SPACE))
		onPrimaryAction();

	if (eventSystem->isPressed(Key::ESC))
		running = false;
}

void Game::handleMouseEvents() {
	Point2 pos = eventSystem->getMouseDPos();

	// relative x and y are wrong on VM Fedora 20
	// TODO: check on real machine
	// POSSIBLE FIX:  since values are 20000+ for relative, perhaps divide by 1000 or adjust just for VM for now ?

	//cout << pos.x << " " << pos.y << endl;

	camera->lookRight(pos.x * 0.0035f);
	camera->lookUp(-pos.y * 0.0035f);	// - for inverted SDL coords

	if (eventSystem->isPressed(Mouse::BTN_LEFT)) {
		onPrimaryAction();
	}

	if (eventSystem->isPressed(Mouse::BTN_RIGHT)) {
		onSecondaryAction();
	}
}

void Game::onPrimaryAction() {
	bullet->center = camera->getPosition();
	while (selected == NULL && distanceBetween(camera->getPosition(), bullet->center) < 20.0f) {
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
	if (selected != NULL) {
		selected->setLocked(false);
		selected = NULL;
	}
}

void Game::update() {
	Vector3 gravity(0, -0.01f, 0);

	float value = 0.0025f;

	ground->setDistZ(ground->halfDistZ.getZ() * 2 - value);
	ground->move(Vector3(0, 0, -value/2.0f));

	//cout << ground->halfDistZ.getZ() << endl;

	for (auto cube : extraBlocks)
		if (!ground->collidesWith(*cube) && cube->alive && selected != cube)	// kinda gravity for now
			cube->center += gravity;

	if (camera->getPosition().getY() > 0)	// for camera gravity, will be replaced by proper
		camera->moveDown();

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
	if (selected != NULL) {
		for (uint i = 0; i < 3; ++i) {
			for (uint j = 0; j < 5; ++j) {
				if (!blocks[j][i]) {
					Point3 c(getValue(j), i*2.0f, -5.0f);
					if (distanceBetween(c, selected->center) < 5.0f) {
						selected->center = c;
						selected->setLocked(false);
						selected->alive = false;
						mainBlocks.push_back(selected);
						extraBlocks.remove(selected);	// addition
						blocks[j][i] = true;
						selected = NULL;	// or selected.reset() ?
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

void Game::testCollision() {

}

void Game::render() {
	gfx->clearScreen();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Point3 pos = camera->getPosition();
	Point3 look = pos + camera->getDirection();
	Vector3 up = camera->getUP();

	gluLookAt(pos.getX(), pos.getY(), pos.getZ(),
		look.getX(), look.getY(), look.getZ(),
		up.getX(), up.getY(), up.getZ());

	ground->draw();
	prize->draw();

	for (auto cube : mainBlocks)
		cube->draw();

	for (auto cube : extraBlocks)	// TODO: clean
		if (cube->alive)
			cube->draw();

	gfx->drawUI();

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
				Point3 p(getValue(j), i*2.0f, -5.0f - 4*step);
				mainBlocks.push_back(make_shared<Cube>(p, 2.0f, COLOR_BLUE));
			}
		}
	}

	for (uint i = 0; i < numberOfBlocksRequired(); ++i) {
		Point3 p(getRandom(-4, 4)*1.0f, getRandom(5, 10)*1.0f, getRandom(25, 35)*1.0f - 5.0f*step);
		extraBlocks.push_back(make_shared<Cube>(p, 2.0f, COLOR_RED));
	}
}