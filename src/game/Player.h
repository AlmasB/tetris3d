#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameObject.h"

class Player : public GameObject {
	private:
		int score, lives;
	public:
		Player(const Point3f & center);
		void addScore(int);
		void addLives(int);
		int getScore() { return score; }
		int getLives() { return lives; }
};

#endif
