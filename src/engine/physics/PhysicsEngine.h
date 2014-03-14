#ifndef __PHYSICS_ENGINE_H__
#define __PHYSICS_ENGINE_H__

#include "../math/GameMath.h"

static const float DEFAULT_GRAVITY = -1.0f;

class PhysicsEngine {
	friend class GameEngine;
	friend class PhysicsObject;
private:
	Vector3f gravity;
	PhysicsEngine();
public:
	/**
	* Note that gravity is naturally a negative value
	* update interval in seconds
	*/
	void setGravity(float gravityValue, float worldUpdateInterval);
};

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
		/**
		* If we have different implementations of engines/gravity
		* this can be very useful
		*/
		virtual void applyGravity(const PhysicsEngine & engine);
		virtual void applyAntiGravity(const PhysicsEngine & engine);
};

#endif