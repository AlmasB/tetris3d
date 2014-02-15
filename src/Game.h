#ifndef __GAME_H__
#define __GAME_H__

#include <iostream>

#include "GraphicsEngine.h"
#include "EventEngine.h"
#include "GameObject.h"

using namespace std;	// for debugging

class Game {
	private:
		GraphicsEngine * gfx;
		Camera * camera;
		EventEngine * eventSystem;

		HPlane * ground = new HPlane(Point3(0, -1, 0), 10, 0, 100);

		Cube * b1 = new Cube(Point3(0, 5, 0), 4.0f);
		Cube * b2 = new Cube(Point3(4, 6, -6), 5.0f);

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