#include "Game.h"

Game::Game() : running(true), gTest(true) {
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
}

void Game::update() {
	Vector3 gravity(0, -0.01f, 0);

	if (gTest) {
		if (b1->center.getY() > 0)
			b1->center += gravity;
		if (b2->center.getY() > 0)
			b2->center += gravity;
		if (b3->center.getY() > 0)
			b3->center += gravity;
	}

	if (camera->getPosition().getY() > 0)
		camera->moveDown();

	testCollision();
}

void Game::testCollision() {
	if (b1->collidesWith(*b2)) {
		std::cout << "b1 collides with b2" << std::endl;
	}

	if (b2->collidesWith(*b3)) {
		std::cout << "b2 collides with b3" << std::endl;
	}

	if (b1->collidesWith(*b3)) {
		if (gTest)
			std::cout << "b1 collides with b3" << std::endl;
		gTest = false;
	}
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

	// some plane below
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