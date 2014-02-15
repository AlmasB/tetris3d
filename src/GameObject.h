#ifndef __GAME_OBJ_H__
#define __GAME_OBJ_H__

#include "BoundingBox.h"

class GameObject : public BoundingBox {
	private:
	protected:
	public:
		GameObject(const Point3 &, float, float, float);
		//virtual void draw() = 0;	// revisit that
};

class Cube : public GameObject {
	public:
		Cube(const Point3 &, float);
		void draw();
};

class HorizontalPlane : public GameObject {
	public:
		HorizontalPlane(const Point3 &, float, float, float);
		bool collidesWith(const BoundingBox &);
		void draw();
};

typedef HorizontalPlane HPlane;

#endif