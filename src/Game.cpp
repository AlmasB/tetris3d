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

Game::Game() : running(true), gTest(true) {
	camera = new Camera();
	gfx = new GraphicsEngine();
	eventSystem = new EventEngine();

	srand(0);

	/*cubes.push_back(new Cube(Point3(-8, 5, 0), 4.0f, COLOR_RED));
	cubes.push_back(new Cube(Point3(4, 6, -6), 4.0f, COLOR_BLUE));
	cubes.push_back(new Cube(Point3(0, 9, -10), 2.0f, COLOR_RED));
	cubes.push_back(new Cube(Point3(15, 5, -3), 2.0f, COLOR_RED));
	cubes.push_back(new Cube(Point3(3, 10, 15), 3.0f, COLOR_BLUE));*/

	// where do we want to "actually" draw the ground line 0,0,0 ?
	// then change values there cube 0,1,0 and 2.0f makes sense a bit more then 0,0,0, 2.0f

	for (uint i = 0; i < 3; ++i) {
		for (uint j = 0; j < 5; ++j) {
			blocks[j][i] = rand() % 2 == 1;
			if (blocks[j][i]) {
				cubes.push_back(new Cube(Point3(getValue(j),i*2.0f,-5.0f), 2.0f, COLOR_BLUE));
			}
		}
	}


	cubes.push_back(new Cube(Point3(0, 0.5f, -48.5), 3.0f, COLOR_AQUA));

	/*cubes.push_back(new Cube(Point3(0, 0, -5), 2.0f, COLOR_BLUE));
	cubes.push_back(new Cube(Point3(0, 2, -5), 2.0f, COLOR_RED));
	cubes.push_back(new Cube(Point3(0, 4, -5), 2.0f, COLOR_BLUE)); */

	extraCubes.push_back(new Cube(Point3(-4, 5, 30), 2.0f, COLOR_RED));
	extraCubes.push_back(new Cube(Point3(-2, 6, 25), 2.0f, COLOR_RED));
	extraCubes.push_back(new Cube(Point3(4, 7, 27), 2.0f, COLOR_RED));
	extraCubes.push_back(new Cube(Point3(3, 8, 35), 2.0f, COLOR_RED));

	bullet = new Cube(Point3(0, 0, 0), 2.0f, COLOR_YELLOW);	// invisible anyway
	bullet->alive = false;

	selected = NULL;
}

Game::~Game() {

	// TODO: clean after everything is written

	for (auto cube : cubes)
		delete cube;

	delete ground;

	delete camera;
	delete eventSystem;
	delete gfx;
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
		if (eventSystem->isPressed(Key::SPACE))
			camera->moveUp();
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

	if (eventSystem->isPressed(Key::ESC))
		running = false;
}

void Game::handleMouseEvents() {
	Point2 pos = eventSystem->getMouseDPos();

	// relative x and y are wrong on VM Fedora 20
	// TODO: check on real machine

	cout << pos.x << " " << pos.y << endl;

	camera->lookRight(pos.x * 0.0035f);
	camera->lookUp(-pos.y * 0.0035f);	// - for inverted SDL coords

	if (eventSystem->isPressed(Mouse::BTN_LEFT)) {
		bullet->center = camera->getPosition();
		//bullet->alive = true;
		while (selected == NULL && distanceBetween(camera->getPosition(), bullet->center) < 25.0f) {
			bullet->move(camera->getDirection());
			for (auto cube : extraCubes) {
				if (bullet->collidesWith(*cube)) {
					cube->setLocked(true);
					//bullet->alive = false;
					selected = cube;
					break;
				}
			}
		}
	}

	if (eventSystem->isPressed(Mouse::BTN_RIGHT)) {
		if (selected != NULL) {
			selected->setLocked(false);
			selected = NULL;
		}
	}
}

void Game::update() {
	Vector3 gravity(0, -0.01f, 0);

	//ground->setDistZ(ground->halfDistZ.getZ() * 2 - 0.1f);
	//ground->move(Vector3(0, 0, -0.05f));

	//cout << ground->halfDistZ.getZ() << endl;

	for (auto cube : extraCubes)
		if (!ground->collidesWith(*cube) && selected != cube)	// kinda gravity for now
			cube->center += gravity;

	if (camera->getPosition().getY() > 0)	// for camera gravity, will be replaced by proper
		camera->moveDown();
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

	for (auto cube : cubes)
		cube->draw();

	for (auto cube : extraCubes)
		cube->draw();

	gfx->drawUI();

	gfx->showScreen();
}
