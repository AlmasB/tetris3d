#include "TestGame.h"

TestGame::TestGame() : AbstractGame() {
	gfx->setWindowSize(1280, 720);
	gfx->setWindowTitle("Test Game");

	std::shared_ptr<Cube> dummy = std::make_shared<Cube>(Point3f(0, 0, -20.0f), 0.5f, 0);
	camera->follow(dummy);
}

TestGame::~TestGame() {

}

void TestGame::handleKeyEvents() {
	if (eventSystem->isPressed(Key::W)) {
		std::cout << "W is pressed" << std::endl;
	}
}

void TestGame::handleMouseEvents() {
	if (eventSystem->isPressed(Mouse::BTN_LEFT)) {
		std::cout << "Left mouse button is pressed" << std::endl;
	}
}

void TestGame::update() {
	//std::cout << "UPDATE" << std::endl;
}

void TestGame::render() {
	Cube c(Point3f(5.0f, 0, 0), 3.0f, SDL_COLOR_BLUE);
	Cube c2(Point3f(0, 0, -5.0f), 3.0f, SDL_COLOR_RED);
	Cube c3(Point3f(-6.0f, 0, 5.0f), 3.0f, SDL_COLOR_GREEN);

	c.draw();
	c2.draw();
	c3.draw();
}