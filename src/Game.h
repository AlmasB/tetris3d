#ifndef __GAME_H__
#define __GAME_H__

#include <iostream>
#include <vector>
#include <list>
#include <memory>

#include "engine/EventEngine.h"
#include "Player.h"
#include "engine/ResourceManager.h"
#include "engine/GraphicsEngine.h"
#include "engine/Timer.h"
#include "Level.h"

#define _RES_TEX_BRICK "res/brick.png"
#define _RES_TEX_PRIZE "res/prize.png"
#define _RES_TEX_WALL "res/wall.png"
#define _RES_TEX_DOORUP "res/doorup.png"
#define _RES_TEX_DOORDOWN "res/doordown.png"
#define _RES_FONT "res/tetris.ttf"


#define __FONT_SIZE 18

#define __APP_FPS 60
#define __BULLET_DISTANCE 20

#define __SCORE_PER_BLOCK 200
#define __SCORE_PER_STEP 1000
#define __SCORE_PER_LEVEL 5000

using namespace std;	// for debugging

static const uint GAME_FPS_DELAY = 1000 / __APP_FPS;

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
		shared_ptr<GameObject> bullet;

		/**
		* Currently selected ("grabbed by player") object
		*/
		shared_ptr<GameObject> selected;

		/**
		* The treasure, the level is won
		* when player gets it ("collides with it") 
		*/
		shared_ptr<GameObject> prize;

		/**
		* UI
		*/
		shared_ptr<GameObject> scoreboard;

		/**
		* ground platforms
		*/
		list<shared_ptr<GameObject>> platforms;

		/**
		* Obstacles
		*/
		list<shared_ptr<GameObject>> mainBlocks;

		/**
		* Additional block pieces
		*/
		list<shared_ptr<GameObject>> extraBlocks;

		/**
		* This can be used when creating cut scenes
		* or when the camera needs to show something
		* but there's no need to move player there
		*
		* This object isn't meant to be drawn
		*/
		shared_ptr<Movable> dummyCameraObject;

		/**
		* Contains x,y values of the obstacle's blocks
		* which are missing
		*/
		list<Point3f> freeBlockSlots;

		///////////////////////////////////// CLEAN //////////////////////////////////////


		list<Point2> openPlatforms;

		void buildPlatforms();
		list<Point2> getNeighborPlatforms(Point2 current);

		void buildBlock();

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
		void updateUI();

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
