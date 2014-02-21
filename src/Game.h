#ifndef __GAME_H__
#define __GAME_H__

#include <iostream>
#include <vector>
#include <list>
#include <memory>

#include "EventEngine.h"
#include "GameObject.h"
#include "Player.h"
#include "GraphicsEngine.h"
#include "Timer.h"

#define __APP_FPS 60

using namespace std;	// for debugging

static const uint GAME_FPS_DELAY = 1000 / __APP_FPS;

enum CutScene {
	NONE, BEGINNING, PLAYER_DEATH, END
};

class Game {
	private:
		unique_ptr<GraphicsEngine> gfx;
		unique_ptr<EventEngine> eventSystem;
		Camera * camera;

		// game objects
		shared_ptr<Player> player;
		shared_ptr<Plane> ground;

		shared_ptr<Cube> prize;
		shared_ptr<Cube> bullet;
		shared_ptr<Cube> selected;

		bool blocks[5][3];

		list<shared_ptr<Cube>> mainBlocks;
		list<shared_ptr<Cube>> extraBlocks;

		list<shared_ptr<Plane>> platforms;

		/////////////////////////////////////
		//test

		list<Point2> openPlatforms;
		list<Point2> openPlatforms2;

		bool isFull();
		RGBColor getRandomColor();

		//int x, y;
		Point2 currentNode;
		bool platformsArray[5][25];
		void buildPlatforms();
		void getNeighborPlatforms();


		//////////////////////////////////////

		bool running;

		uint step;

		void buildBlock();
		bool isGameWon();
		void newBlocks();

		void onPrimaryAction();
		void onSecondaryAction();

		uint numberOfBlocksRequired();


		/* CUT-SCENES RELATED STUFF */
		CutScene currentCutScene;
		uint cutSceneFrame;
		Timer cutSceneTimer;

		void playCutScene();
		void playCutSceneBeginning();
		void playCutScenePlayerDeath();
		void playCutSceneEnd();


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
