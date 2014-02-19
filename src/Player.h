#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameObject.h"

class Player : public Cube {
	private:
		uint score;
	public:
		Player(const Point3f & center);
};



#endif
