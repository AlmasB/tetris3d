#include "Game.h"

Game::Game() : running(true) {
	camera = new Camera();
	gfx = new GraphicsEngine();
	eventSystem = new EventEngine();
}

Game::~Game() {
	delete b1;
	delete b2;
	delete b3;
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



		update();
		render();

		end = SDL_GetTicks() - start;
		if (end < 20) {
			SDL_Delay(20 - end);
		}
	}
}

void Game::update() {
	Vector3 gravity(0, -0.01f, 0);

	if (eventSystem->isPressed(Key::W))
		camera->moveForward();
	if (eventSystem->isPressed(Key::S))
		camera->moveBack();
	if (eventSystem->isPressed(Key::A))
		camera->moveLeft();
	if (eventSystem->isPressed(Key::D))
		camera->moveRight();
	if (eventSystem->isPressed(Key::ESC))
		running = false;

	int mx, my;
	SDL_GetRelativeMouseState(&mx, &my);

	camera->rotateRight(mx * 0.0035f);
	camera->rotateUp(-my * 0.0035f);


	if (b1->center.getY() > 0)
		b1->center += gravity;
	if (b2->center.getY() > 0)
		b2->center += gravity;
	if (b3->center.getY() > 0)
		b3->center += gravity;
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

	// some plane underneath
	glColor3f(1.0f, 0.9f, 1.0f);

	glBegin(GL_QUADS);
	glVertex3f(-100.0f, -1.0f, -100.0f);
	glVertex3f(-100.0f, -1.0f, 100.0f);
	glVertex3f(100.0f, -1.0f, 100.0f);
	glVertex3f(100.0f, -1.0f, -100.0f);
	glEnd();

	glColor3f(0.5f, 0.5f, 0.2f);
	b1->draw();
	b2->draw();
	b3->draw();

	gfx->drawUI();

	gfx->showScreen();
}