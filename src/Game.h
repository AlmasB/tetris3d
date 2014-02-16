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

		HPlane * ground = new HPlane(Point3(0, -1, 0), 10, 0, 100, COLOR_GRAY);	// reconsider ground Y

		Cube * bullet;
		Cube * selected;

		bool blocks[5][3];

		vector<Cube*> cubes;
		vector<Cube*> extraCubes;

		bool running;
		bool gTest;

		void buildBlock();
		bool isGameWon();
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