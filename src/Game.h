#ifndef __GAME_H__
#define __GAME_H__

#include <iostream>
#include <vector>
#include <list>
#include <memory>

#include "EventEngine.h"
#include "GameObject.h"
#include "Player.h"
#include "ResourceManager.h"
#include "GraphicsEngine.h"
#include "Timer.h"
#include "Level.h"

#define _RES_TEX_BRICK "res/brick.png"
#define _RES_TEX_PRIZE "res/prize.png"
#define _RES_FONT "res/tetris.ttf"


#define __FONT_SIZE 18

#define __APP_FPS 60
#define __BULLET_DISTANCE 20

#define __SCORE_PER_BLOCK 200
#define __SCORE_PER_STEP 1000
#define __SCORE_PER_LEVEL 5000

using namespace std;	// for debugging

static const uint GAME_FPS_DELAY = 1000 / __APP_FPS;

static const SDL_Color SDL_COLOR_RED = { 255, 0, 0 };

enum CutScene {
	NONE, LEVEL_BEGINNING, LEVEL_END, PLAYER_DEATH, GAME_WIN, GAME_LOSE
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

		shared_ptr<Plane> scoreboard;

		shared_ptr<Plane> wall1;
		shared_ptr<Plane> wall2;

		/**
		* ground platforms
		*/
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
		void buildPlatforms();
		void getNeighborPlatforms();

		void buildBlock();

		GLuint textureBrick;

		//////////////////////////////////////////////////////////////////////////////////

		/* GAMEPLAY STUFF */
		bool running;	// main loop control
		void nextLevel();
		bool isLevelBuilt();
		shared_ptr<Level> currentLevel;
		
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

		/**
		* These can control flow of the cutscene,
		* its onStart() and its onFinish()
		* Both can be used simultaneously
		*/
		uint cutSceneFrame;		// records how much frames passed since FIRST call to this cutscene
		Timer cutSceneTimer;	// records how much time passed since LAST call to this cutscene

		/**
		* Each cut scene ideally should have onStart() and onFinish() 
		* sort of thing within them
		*/
		void playCutScene();
		void playCutSceneLevelBeginning();
		void playCutSceneLevelEnd();
		void playCutScenePlayerDeath();
		void playCutSceneGameWin();
		void playCutSceneGameLose();
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
