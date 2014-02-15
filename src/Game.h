#ifndef __GAME_H__
#define __GAME_H__

#include <iostream>

#include "GraphicsEngine.h"
#include "EventEngine.h"
#include "Box.h"


class Game {
	private:
		GraphicsEngine * gfx;
		Camera * camera;
		EventEngine * eventSystem;

		Box * b1 = new Box(5, 3, -7);
		Box * b2 = new Box(0, 0, -10);
		Box * b3 = new Box(4, 6, -6);

		bool running;
		bool gTest;
	public:
		Game();
		~Game();

		bool init();
		void runMainLoop();

		void handleKeyEvents();
		void handleMouseEvents();

		void update();
		void render();

		void testCollision();
};


#endif