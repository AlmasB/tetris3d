#ifndef __GAME_H__
#define __GAME_H__

#include <iostream>
#include <vector>
#include <list>
#include <memory>

#include "../engine/core/GameEngine.h"
#include "Player.h"
#include "../engine/graphics/MD3Object.h"
#include "Level.h"

#define _TETRIS_VERSION_MAJOR 0
#define _TETRIS_VERSION_MINOR 2
#define _TETRIS_AUTHOR "Almas"

/**
 * RESOURCE FILES
 * To avoid typos/renames and other issues in the code
 */
#define _RES_TEX_BRICK		"res/brick.png"
#define _RES_TEX_PRIZE		"res/prize.png"
#define _RES_TEX_CROSSHAIR	"res/crosshair.png"

#define _RES_SFX_CLONG		"res/audio/clong.wav"

#define _RES_FONT			"res/tetris.ttf"

using namespace std;	// for debugging

static const int GAME_W = 800;
static const int GAME_H = 600;

static const Point3f WORLD_ORIGIN = Point3f(0, 0, 0);

enum CutScene {
	NONE, LEVEL_BEGINNING, LEVEL_END, PLAYER_DEATH, GAME_WIN
};

class Game {
	private:
		/** 
		 * GAMEPLAY CONSTANTS
		 * Adjust as needed
		 */
		static const int SCORE_PER_BLOCK = 200;
		static const int SCORE_PER_STEP = 1000;
		static const int SCORE_PER_LEVEL = 5000;

		static const int BULLET_DISTANCE = 20;

		/* TECHNICAL CONSTANTS */
		static const int GAME_FPS = 60;
		static const int GAME_FPS_DELAY_MSEC = __SECOND / GAME_FPS;
		static const float GAME_FPS_DELAY_SEC;	// must be inited in .cpp

		/* ENGINE OBJECTS */
		shared_ptr<GraphicsEngine> gfx;
		shared_ptr<AudioEngine> sfx;
		shared_ptr<EventEngine> eventSystem;
		shared_ptr<Camera> camera;

		/* GAME OBJECTS */
		shared_ptr<Player> player;
		shared_ptr<GameObject> crosshair;

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

		MD3Object * dummy;

		list<Point2> openPlatforms;
		list<Point2> getNeighborPlatforms(Point2 current);

		/**
		* Builds/destroy a certain amount of platforms at a time
		* until there are no more platforms
		*/
		void buildPlatforms();
		void killPlatform();
		
		void buildBlock();

		//////////////////////////////////////////////////////////////////////////////////

		/* GAMEPLAY STUFF */
		bool running;	// main loop control
		bool god;       // god mode for easy debug (can't die, no player collision)
		Timer worldTimer;
		void initLevel();
		void nextLevel();
		bool isLevelBuilt();
		shared_ptr<Level> currentLevel;
		
		/**
		* Level consists of several steps
		* Pushes (new) obstacles towards the treasure
		*/
		void nextStep();
		int currentStep;

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
		* Both timer and frame can be used simultaneously
		*/
		uint cutSceneFrame;		// records how much FRAMES passed since FIRST call to this cutscene
		Timer cutSceneTimer;	// records how much TIME passed since LAST call to this cutscene

		/**
		* Each cut scene ideally should have onStart() and onFinish() 
		* sort of thing within them
		*/
		void playCutScene();
		void playCutSceneLevelBeginning();
		void playCutSceneLevelEnd();
		void playCutScenePlayerDeath();
		void playCutSceneGameWin();
		void resetCutScene();

		/* EVENTS & UPDATES */
		/**
		* Handle all generated and currently active events
		* Includes keyboard/mouse events
		*/
		void handleAllEvents();
		void handleKeyEvents();
		void handleMouseEvents();
		void handlePlayerMovement();

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

		void runMainLoop();
};

#endif
