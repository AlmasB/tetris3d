#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameObject.h"

class Player : public Cube {
	private:
		uint score, lives;
	public:
		Player(const Point3f & center);
		void addScore(uint);
		void addLives(int);
		uint getScore();
		uint getLives();
};



#endif
