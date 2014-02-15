#ifndef __GAME_H__
#define __GAME_H__

#include <iostream>
#include <vector>

#include "GraphicsEngine.h"
#include "EventEngine.h"
#include "GameObject.h"

using namespace std;	// for debugging



class Game {
	private:
		GraphicsEngine * gfx;
		Camera * camera;
		EventEngine * eventSystem;

		HPlane * ground = new HPlane(Point3(0, -1, 0), 10, 0, 100, COLOR_GRAY);

		Cube * bullet;
		Cube * selected;

		vector<Cube*> cubes;

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