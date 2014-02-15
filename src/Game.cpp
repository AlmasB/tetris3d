#include "Game.h"

Game::Game() : running(true), gTest(true) {
	camera = new Camera();
	gfx = new GraphicsEngine();
	eventSystem = new EventEngine();

	cubes.push_back(new Cube(Point3(-8, 5, 0), 4.0f, COLOR_RED));
	cubes.push_back(new Cube(Point3(4, 6, -6), 5.0f, COLOR_GREEN));
	cubes.push_back(new Cube(Point3(0, 9, -10), 2.0f, COLOR_GREEN));
	cubes.push_back(new Cube(Point3(15, 5, -3), 2.0f, COLOR_RED));
	cubes.push_back(new Cube(Point3(3, 10, 15), 3.0f, COLOR_BLUE));
}

Game::~Game() {

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
	if (eventSystem->isPressed(Key::ESC))
		running = false;
}

void Game::handleMouseEvents() {
	Point2 pos = eventSystem->getMouseDPos();
	camera->lookRight(pos.x * 0.0035f);
	camera->lookUp(-pos.y * 0.0035f);	// - for inverted SDL coords

	if (eventSystem->isPressed(Mouse::BTN_LEFT)) {
		//cout << "Left click" << endl;
	}

	if (eventSystem->isPressed(Mouse::BTN_RIGHT)) {
		//cout << "Right click" << endl;
	}
}

void Game::update() {
	Vector3 gravity(0, -0.01f, 0);

	for (auto cube : cubes)
		if (!ground->collidesWith(*cube))
			cube->center += gravity;

	if (camera->getPosition().getY() > 0)
		camera->moveDown();

	//testCollision();
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

	gfx->drawUI();

	gfx->showScreen();
}