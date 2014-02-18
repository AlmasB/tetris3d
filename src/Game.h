#ifndef __GAME_H__
#define __GAME_H__

#include <iostream>
#include <vector>
#include <list>
#include <memory>	// for shared_ptr, will add later


#include "EventEngine.h"
#include "GameObject.h"
#include "GraphicsEngine.h"

using namespace std;	// for debugging



class Game {
	private:
		unique_ptr<GraphicsEngine> gfx;
		shared_ptr<Camera> camera;
		unique_ptr<EventEngine> eventSystem;

		// game objects
		shared_ptr<HPlane> ground;

		shared_ptr<Cube> prize;
		shared_ptr<Cube> bullet;
		shared_ptr<Cube> selected;

		Cube * c;

		bool blocks[5][3];

		list<shared_ptr<Cube>> mainBlocks;
		list<shared_ptr<Cube>> extraBlocks;

		bool running;
		bool gTest;

		uint step;

		void buildBlock();
		bool isGameWon();
		void newBlocks();

		void onPrimaryAction();
		void onSecondaryAction();

		uint numberOfBlocksRequired();
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
