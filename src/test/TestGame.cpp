#include "TestGame.h"

TestGame::TestGame() : AbstractGame() {
	gfx->setWindowSize(1280, 720);
    //gfx->setFullscreen(true);
	gfx->setWindowTitle("Maze Game");

	dummy = std::make_shared<Cube>(Point3f(0, 0.0f, 0.0f), 0.5f, 0);
	camera->follow(dummy);
    
    eventSystem->setMouseRelative(true);
    
    std::string fileName = std::string("res/BowlingShoes.ttf");
    std::string soundName = std::string("res/audio/clong.wav");
    std::vector<std::string> resourceNames;
    
    resourceNames.push_back(fileName);
    resourceNames.push_back(soundName);
    
    
    ResourceManager::loadResources(resourceNames);
    
    gfx->useFont(ResourceManager::getFont("res/BowlingShoes.ttf"));

	/*gen = new MazeGenerator(50, 40);
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
	}*/
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

void TestGame::onLeftMouseButton() {
    std::cout << "Left mouse Button pushed" << std::endl;
    sfx->playSound(ResourceManager::getSound("res/audio/clong.wav"));
    //gfx->setWindowSize(800, 600);
    
}

void TestGame::update() {
	//std::cout << "UPDATE" << std::endl;
}

void TestGame::render() {
	Cube c(Point3f(2.0f, 0, 0), 3.0f, SDL_COLOR_BLUE);
	Cube c2(Point3f(0, 0, 0.0f), 3.0f, SDL_COLOR_RED);
	Cube c3(Point3f(-6.0f, 0, 5.0f), 3.0f, SDL_COLOR_GREEN);
    
    Cuboid c4(Point3f(0, -5.0f, 0), 1.0f, 3.0f, 0.5f, SDL_COLOR_BLUE);
    
    if (c.isColliding(c2)) {
        std::cout << "colliding" << std::endl;
    }

	c.draw();
	c2.draw();
	c3.draw();
    c4.draw();
    
    std::string text = std::string("Test text");
    
    SDL_Color white = {255, 255, 255, 0};
    SDL_Color white2 = {255, 255, 255, 255};
    
    gfx->drawText(text, white, 50, 50);
    gfx->drawText(text, white2, 350, 50);
    
	//for (auto wall : walls)
		//wall->draw();
	
}