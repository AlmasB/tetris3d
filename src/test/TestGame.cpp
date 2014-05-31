#include "TestGame.h"

TestGame::TestGame() : AbstractGame() {
	gfx->setWindowSize(1280, 720);
	//gfx->setWindowSize(1920, 1080);
    //gfx->setFullscreen(true);
	gfx->setWindowTitle("Maze Game");

	dummy = std::make_shared<Cube>(Point3f(30.0f, 2.0f, 0.0f), 0.5f, 0);
	//sky = std::make_shared<Cube>(Point3f(0, 2.0f, 35.0f), 30.0f, GraphicsEngine::createGLTextureFromSurface(IMG_Load("res/skybox/bot.png")));
	camera->follow(dummy);
    
    eventSystem->setMouseRelative(true);
    
    std::string fileName = std::string("res/tetris.ttf");
    std::string soundName = std::string("res/audio/clong.wav");
	std::string mp3Name = "res/audio/test0.mp3";
    std::vector<std::string> resourceNames;
    
    resourceNames.push_back(fileName);
    resourceNames.push_back(soundName);
	resourceNames.push_back(mp3Name);
	resourceNames.push_back("res/wall2048.png");
	resourceNames.push_back("res/brick.png");
    
    ResourceManager::loadResources(resourceNames);
    
    gfx->useFont(ResourceManager::getFont("res/tetris.ttf"));

	
	float size = 1000.0f;
	float distance = 500.0f;

	std::shared_ptr<Cuboid> bot = std::make_shared<Cuboid>(Point3f(0, -distance+5, 0), size+10, size+10, 0.5f, GraphicsEngine::createGLTextureFromSurface(IMG_Load("res/skybox/bot.png")));
	bot->rotate(90, 0, 0);

	std::shared_ptr<Cuboid> top = std::make_shared<Cuboid>(Point3f(0, distance-5, 0), size+10, size+10, 0.5f, GraphicsEngine::createGLTextureFromSurface(IMG_Load("res/skybox/top.png")));
	top->rotate(-90, 0, 0);

	std::shared_ptr<Cuboid> left = std::make_shared<Cuboid>(Point3f(-distance, 0.0f, 0), size+1, size+1, 0.5f, GraphicsEngine::createGLTextureFromSurface(IMG_Load("res/skybox/left.png")));
	left->rotate(0, 90, 0);

	std::shared_ptr<Cuboid> right = std::make_shared<Cuboid>(Point3f(distance, 0.0f, 0.0f), size+1, size+1, 0.5f, GraphicsEngine::createGLTextureFromSurface(IMG_Load("res/skybox/right.png")));
	right->rotate(0, -90, 0);

	std::shared_ptr<Cuboid> front = std::make_shared<Cuboid>(Point3f(0.0f, 0.0f, distance), size+1, size+1, 0.5f, GraphicsEngine::createGLTextureFromSurface(IMG_Load("res/skybox/front.png")));
	std::shared_ptr<Cuboid> back = std::make_shared<Cuboid>(Point3f(0.0f, 0.0f, -distance), size+1, size+1, 0.5f, GraphicsEngine::createGLTextureFromSurface(IMG_Load("res/skybox/back.png")));




	boxes.push_back(bot);
	boxes.push_back(top);
	boxes.push_back(left);
	boxes.push_back(right);
	boxes.push_back(front);
	boxes.push_back(back);




	ground = std::make_shared<Cuboid>(Point3f(0, 7.0f, 15.0f), 50.0f, 15.0f, 5.0f, SDL_COLOR_GRAY);




	// walls
	//walls.push_back(std::make_shared<Cuboid>(Point3f(0, 7.0f, 15.0f), 50.0f, 15.0f, 5.0f, SDL_COLOR_RED));

	//sfx->playMP3(ResourceManager::getMP3("res/audio/test0.mp3"), -1);

	gen = new MazeGenerator(50, 40);
	gen->generateMaze(0, 0);
	gen->display();

	eventSystem->setMouseRelative(true);

	float wallY = 10.0f;
	float dist = 26.0f;
	GLuint texture = ResourceManager::getTextureID("res/wall2048.png");

	for (int j = 0; j < 10; ++j) {
		for (int i = 0; i < 10; ++i) {
			walls.push_back(std::make_shared<Cuboid>(Point3f(j*2, -0.1f, i*2), 2.0f, 0.2f, 2.0f, ResourceManager::getTextureID("res/brick.png")));
		}
	}

	/*for (int j = 0; j < 10; ++j) {
		for (int i = 0; i < 10; ++i) {
			objects.push_back(std::make_shared<Cuboid>(Point3f(i * 2, -0.1f + j, 2.0f), 2.0f, 1.0f, 1.0f, SDL_COLOR_GRAY));
		}
	}*/

	test = std::make_shared<Cuboid>(Point3f(5.0f, 1.0f, 5.0f), 2.0f, 2.0f, 2.0f, SDL_COLOR_BLUE);
	test2 = std::make_shared<Cuboid>(Point3f(50.0f, 1.5f, 5.0f), 3.0f, 3.0f, 3.0f, SDL_COLOR_RED);


	//walls.push_back(std::make_shared<Cuboid>(Point3f(0, 12.0f, 0), 100.0f, 3.0f, 200.0f, SDL_COLOR_GRAY));

	/*for (int i = 0; i < gen->y; ++i) {	//y
		for (int j = 0; j < gen->x; ++j) {	//x
			if ((gen->maze[j][i] & 1) == 0) {
				std::shared_ptr<Cuboid> c = std::make_shared<Cuboid>(Point3f(j * dist, 0, i * dist), dist, wallY, 0.5f, texture);
				walls.push_back(c);
			}

			if ((gen->maze[j][i] & 8) == 0) {
				std::shared_ptr<Cuboid> c = std::make_shared<Cuboid>(Point3f(j * dist - dist / 2, 0, i * dist + dist/2), dist, wallY, 0.5f, texture);
				c->rotate(0, 90, 0);
				walls.push_back(c);
			}
		}

		std::shared_ptr<Cuboid> c = std::make_shared<Cuboid>(Point3f(gen->x * dist - dist/2, 0, i * dist + dist/2), dist, wallY, 0.5f, texture);
		c->rotate(0, 90, 0);
		walls.push_back(c);
	}

	for (int j = 0; j < gen->x; ++j) {
		std::shared_ptr<Cuboid> c = std::make_shared<Cuboid>(Point3f(j * dist, 0, gen->y * dist), dist, wallY, 0.5f, texture);
		walls.push_back(c);
	}*/

	sphere = std::make_shared<Sphere>(Point3f(0, 0, 15.0f), 2.0f, SDL_COLOR_AQUA);

	mass = 1.0f;
	time = 0.0f;
	force = 10;
	v1 = 0.016 * force / mass;
	v2 = 0.016 * force / mass;
	collide = false;
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

	if (eventSystem->isPressed(Key::SPACE)) {
	}

	Point2 pos = eventSystem->getMouseDPos();

	dummy->lookRight((float)pos.x);
	dummy->lookUp((float)-pos.y);	// invert, because UP is negative in SDL

	
}

void TestGame::onLeftMouseButton() {
    std::cout << "Left mouse Button pushed" << std::endl;
    sfx->playSound(ResourceManager::getSound("res/audio/clong.wav"));
}

void TestGame::update() {
	/*for (auto wall : walls) {
		if (distanceBetween(wall->getCenter(), dummy->getCenter()) < 30.0f) {
			wall->intensity = 1.0f;
		}
		else {
			wall->intensity = 30 / distanceBetween(wall->getCenter(), dummy->getCenter());
		}
	}*/

	// gravity
	/*if (test->getCenter().y - test->getHalfLengthY() > 1)
		test->move(Vector3f(0, -9.8f * time, 0));

	if (test2->getCenter().y - test2->getHalfLengthY() > 1)
		test2->move(Vector3f(0, -9.8f * time, 0));*/

	if (!collide) {
		test->move(Vector3f(v1, 0, 0));
		test2->move(Vector3f(-v2, 0, 0));
		if (test->isColliding(*test2)) {
			collide = true;
		}
	}
	else {
		test->move(Vector3f(-v1, 0, 0));
		test2->move(Vector3f(v2, 0, 0));

		if (v1 > 0)
			v1 -= 0.001;
		else
			v1 = 0;

		if (v2 > 0)
			v2 -= 0.001;
		else
			v2 = 0;
	}


	/*if (time < 5) {

		for (auto obj : objects) {
			obj->move(Vector3f(getRandom(0, 1)*0.16, obj->getCenter().y - obj->getHalfLengthY() > 1 ? -0.016 : 0, getRandom(0, 2)*0.16));
		}
	}*/

	time += 0.016;
}

void TestGame::render() {
	/*Cube c(Point3f(5.0f, 0, 0), 3.0f, SDL_COLOR_ORANGE);
	Cube c2(Point3f(0, 0, 10.0f), 3.0f, SDL_COLOR_RED);
	Cube c3(Point3f(-6.0f, 0, 5.0f), 3.0f, SDL_COLOR_GREEN);
    
    Cuboid c4(Point3f(0, -5.0f, 0), 1.0f, 3.0f, 0.5f, SDL_COLOR_PINK);
    
	c.draw();
	c2.draw();
	c3.draw();
    c4.draw();*/

	//sphere->draw();

	

	for (auto box : boxes)
		box->draw();

	//ground->draw();

	for (auto wall : walls)
		wall->draw();

	test->draw();
	test2->draw();

	for (auto obj : objects)
		obj->draw();

	//sky->draw();

    /*std::string text = std::string("Test text");
    
    SDL_Color white = {255, 255, 255, 0};
    SDL_Color white2 = {255, 255, 255, 255};
    
    gfx->drawText(text, white, 50, 50);
    gfx->drawText(text, white2, 350, 50);

	gfx->drawText("Violet", SDL_COLOR_VIOLET, 100, 100);
	gfx->drawText("Pink", SDL_COLOR_PINK, 250, 150);
	gfx->drawText("Aqua", SDL_COLOR_AQUA, 400, 200);
	gfx->drawText("Orange", SDL_COLOR_ORANGE, 550, 250);*/
}