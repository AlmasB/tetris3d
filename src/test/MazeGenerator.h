#ifndef __X__D__
#define __X__D__

#include <cstdlib>
#include <iostream>

#include "../engine/math/GameMath.h"

class Dir;

class Dir {
	public:
		Dir();
		Dir(Dir &);
		int bit, dx, dy;
		Dir * opposite;
};

class MazeGenerator {
	private:
		
		Dir N, S, E, W;

		
	public:
		MazeGenerator(const int &, const int &);
		int x, y;
		int ** maze;

		void generateMaze(int, int);
		void display();
};

static bool between(int v, int upper) {
	return v >= 0 && v < upper;
}

#endif