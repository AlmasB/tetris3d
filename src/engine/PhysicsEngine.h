#ifndef __PHYSICS_ENGINE_H__
#define __PHYSICS_ENGINE_H__

#include "GameMath.h"

class PhysicsObject {
	protected:
		Point3f center;
		float lX, lY, lZ, hlX, hlY, hlZ;	// lengths and half lengths
	public:
		PhysicsObject(const Point3f & center, float x, float y, float z);

		Point3f getCenter() { return center; }
		float getLengthX() { return lX; }
		float getLengthY() { return lY; }
		float getLengthZ() { return lZ; }
		float getHalfLengthX() { return hlX; }
		float getHalfLengthY() { return hlY; }
		float getHalfLengthZ() { return hlZ; }

		bool isColliding(const PhysicsObject & other);
};

class PhysicsEngine {


	// setGravity();
};

#endif