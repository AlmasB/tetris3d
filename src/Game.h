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

		/**
		* This can be used when creating cut scenes
		* or when the camera needs to show something
		* but there's no need to move player there
		*
		* This object isn't meant to be drawn
		*/
		shared_ptr<Movable> dummyCameraObject;

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

		list<Point2> openPlatforms;
		list<Point2> openPlatforms2;

		bool isFull();
		RGBColor getRandomColor();

		//int x, y;
		Point2 currentNode;
		bool platformsArray[5][25];
		void buildPlatforms();
		void getNeighborPlatforms();

		bool test;
		int test2;


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

		/**
		* Each cut scene ideally should have onStart() and onFinish() 
		* sort of thing within them
		*/
		void playCutScene();
		void playCutSceneBeginning();
		void playCutScenePlayerDeath();
		void playCutSceneEnd();
		void resetCutScene();


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
