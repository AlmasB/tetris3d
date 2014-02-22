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
#include "Level.h"

#define __APP_FPS 60
#define __BULLET_DISTANCE 20

using namespace std;	// for debugging

static const uint GAME_FPS_DELAY = 1000 / __APP_FPS;

enum CutScene {
	NONE, BEGINNING, PLAYER_DEATH, END
};

class Game {
	private:
		/* ENGINE SUB-SYSTEMS */
		unique_ptr<GraphicsEngine> gfx;
		unique_ptr<EventEngine> eventSystem;
		Camera * camera;

		/* GAME OBJECTS */
		shared_ptr<Player> player;

		/**
		* Invisible entity used to "shoot"
		* game objects to identify which object player
		* wants to grab
		*
		* This object isn't meant to be drawn
		*/
		shared_ptr<Cube> bullet;

		/**
		* Currently selected ("grabbed by player") object
		*/
		shared_ptr<Cube> selected;

		/**
		* The treasure, the level is won
		* when player gets it ("collides with it") 
		*/
		shared_ptr<Cube> prize;

		shared_ptr<Plane> ground;	// ground as a whole will consist of platforms and will be removed
		list<shared_ptr<Plane>> platforms;

		/**
		* Obstacles
		*/
		list<shared_ptr<Cube>> mainBlocks;

		/**
		* Additional block pieces
		*/
		list<shared_ptr<Cube>> extraBlocks;

		/**
		* This can be used when creating cut scenes
		* or when the camera needs to show something
		* but there's no need to move player there
		*
		* This object isn't meant to be drawn
		*/
		shared_ptr<Movable> dummyCameraObject;


		///////////////////////////////////// CLEAN //////////////////////////////////////

		bool blocks[5][3];

		list<Point2> openPlatforms;
		list<Point2> openPlatforms2;

		bool isFull();
		RGBColor getRandomColor();

		Point2 currentNode;
		bool platformsArray[5][25];
		void buildPlatforms();
		void getNeighborPlatforms();

		void buildBlock();

		//////////////////////////////////////////////////////////////////////////////////

		/* GAMEPLAY STUFF */
		bool running;	// main loop control
		void nextLevel();
		//Level currentLevel;
		
		/**
		* Level consists of several steps
		* Pushes (new) obstacles towards the treasure
		*/
		void nextStep();
		uint currentStep;
		bool isStepCompleted();

		uint numberOfBlocksRequired();	// to complete current step's structure

		/**
		* Recreates obstacles and extra blocks
		*/
		void spawnAllBlocks();

		/**
		* Correspond to actions player can do
		*
		* Instead of hardcoding actions these can be called
		* when using any device (e.g. mouse/keyboard/joystick etc)
		*/
		void onPrimaryAction();
		void onSecondaryAction();


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

		/* EVENTS & UPDATES */
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
