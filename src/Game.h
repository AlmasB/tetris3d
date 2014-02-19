#ifndef __GAME_H__
#define __GAME_H__

#include <iostream>
#include <vector>
#include <list>
#include <memory>

#include "EventEngine.h"
#include "GameObject.h"
#include "GraphicsEngine.h"

#define __APP_FPS 60

using namespace std;	// for debugging

static const uint GAME_FPS_DELAY = 1000 / __APP_FPS;

class Game {
	private:
		unique_ptr<GraphicsEngine> gfx;
		unique_ptr<EventEngine> eventSystem;
		Camera * camera;

		// game objects
		shared_ptr<Plane> ground;

		shared_ptr<Cube> prize;
		shared_ptr<Cube> bullet;
		shared_ptr<Cube> selected;

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


		/**
		* Handle all generated and currently active events
		* Includes keyboard/mouse events
		*/
		void handleAllEvents();
		void handleKeyEvents();
		void handleMouseEvents();

		/**
		* Updates the game world
		*/
		void update();

		/**
		* Renders the game world
		*/
		void render();

	public:
		Game();
		~Game();

		bool init();
		void runMainLoop();
};

#endif
