#ifndef __GAME_OBJ_H__
#define __GAME_OBJ_H__

#include "engine/Primitives3d.h"

class GameObject : public Cuboid {
	private:
		bool locked;

	public:
		GameObject(const Point3f & center, float x, float y, float z, SDL_Color color);
		GameObject(const Point3f & center, float x, float y, float z, GLuint textureID);

		void setLocked(bool b);
};


#endif
