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

		Timer worldTimer;

		std::shared_ptr<Cube> sky;
		std::shared_ptr<Sphere> sphere;

		std::shared_ptr<Cuboid> test;
		std::shared_ptr<Cuboid> test2;
		std::shared_ptr<Cuboid> ground;

		std::vector<std::shared_ptr<Cuboid>> walls;
		std::vector<std::shared_ptr<Cuboid>> boxes;

		std::vector<std::shared_ptr<Cuboid>> objects;


		void handleKeyEvents();
        void onLeftMouseButton();
	
		void update();
		void render();


		// physics test

		float mass;
		float time;
		float v1, v2;
		float force;

		bool collide;

	public:
		TestGame();
		~TestGame();
		
};

#endif