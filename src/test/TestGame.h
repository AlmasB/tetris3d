#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/core/AbstractGame.h"
#include "../engine/graphics/Primitives3d.h"
#include "../engine/core/ResourceManager.h"

#include "MazeGenerator.h"

class TestGame : public AbstractGame {
	private:

		MazeGenerator * gen;
		std::shared_ptr<Cube> dummy;

		std::vector<std::shared_ptr<Cuboid>> walls;

		void handleKeyEvents();
        void onLeftMouseButton();
	
		void update();
		void render();
	public:
		TestGame();
		~TestGame();
		
};

#endif