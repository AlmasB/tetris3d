#include "TestGame.h"

TestGame::TestGame() : AbstractGame() {
	gfx->setWindowSize(1280, 720);
	gfx->setWindowTitle("Test Game");

	dummy = std::make_shared<Cube>(Point3f(0, -90.0f, 0.0f), 0.5f, 0);
	camera->follow(dummy);

	gen = new MazeGenerator(50, 40);
	gen->generateMaze(0, 0);
	gen->display();

	eventSystem->setMouseRelative(true);

	for (int i = 0; i < gen->y; ++i) {	//y
		for (int j = 0; j < gen->x; ++j) {	//x
			if ((gen->maze[j][i] & 1) == 0) {
				std::shared_ptr<Cuboid> c = std::make_shared<Cuboid>(Point3f(j*4, 0, i*4), 4.0f, 3.0f, 0.5f, SDL_COLOR_GREEN);
				walls.push_back(c);
			}

			if ((gen->maze[j][i] & 8) == 0) {
				std::shared_ptr<Cuboid> c = std::make_shared<Cuboid>(Point3f(j*4-2, 0, i*4+2), 4.0f, 3.0f, 0.5f, SDL_COLOR_BLUE);
				c->rotate(0, 90, 0);
				walls.push_back(c);
			}
		}

		std::shared_ptr<Cuboid> c = std::make_shared<Cuboid>(Point3f(gen->x * 4 - 2, 0, i * 4 + 2), 4.0f, 3.0f, 0.5f, SDL_COLOR_BLUE);
		c->rotate(0, 90, 0);
		walls.push_back(c);
	}

	for (int j = 0; j < gen->x; ++j) {
		std::shared_ptr<Cuboid> c = std::make_shared<Cuboid>(Point3f(j * 4, 0, gen->y * 4), 4.0f, 3.0f, 0.5f, SDL_COLOR_GREEN);
		walls.push_back(c);
	}
}

TestGame::~TestGame() {

}

void TestGame::handleKeyEvents() {
	if (eventSystem->isPressed(Key::W)) {
		dummy->moveForward();
	}

	if (eventSystem->isPressed(Key::S)) {
		dummy->moveBackward();
	}

	if (eventSystem->isPressed(Key::A)) {
		dummy->moveLeft(0.15f);
	}

	if (eventSystem->isPressed(Key::D)) {
		dummy->moveRight(0.15f);
	}

	Point2 pos = eventSystem->getMouseDPos();

	dummy->lookRight((float)pos.x);
	dummy->lookUp((float)-pos.y);	// invert, because UP is negative in SDL
}

void TestGame::update() {
	//std::cout << "UPDATE" << std::endl;
}

void TestGame::render() {
	/*Cube c(Point3f(5.0f, 0, 0), 3.0f, SDL_COLOR_BLUE);
	Cube c2(Point3f(0, 0, -5.0f), 3.0f, SDL_COLOR_RED);
	Cube c3(Point3f(-6.0f, 0, 5.0f), 3.0f, SDL_COLOR_GREEN);

	c.draw();
	c2.draw();
	c3.draw();*/
	for (auto wall : walls)
		wall->draw();
	
}