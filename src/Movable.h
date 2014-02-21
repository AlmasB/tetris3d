#ifndef __MOVABLE_H__
#define __MOVABLE_H__

#include "GameMath.h"
#include "Debug.h"

#if defined(__DEBUG_ENABLED)
	#define __DIRECTION 1
	#define __CENTER 2
	#define __UP 4
#endif

class Movable {
	protected:
		Vector3f direction;
		Vector3f up;

		float horAngle, verAngle;
		float speed;

	public:
		Movable();
		virtual void move(const Vector3f & v) = 0;

		// as if you were looking thru the object's eyes
		void moveForward();
		void moveBackward();
		void moveRight(float speed);
		void moveLeft(float speed);

		void lookUp(float);
		void lookRight(float);
		void lookAt(const Point3f &point);
		void adjustDirection();

		Vector3f getDirection();
		Vector3f getUpVector();
		virtual Point3f getCenter() = 0;

#if defined(__DEBUG_ENABLED)
		void printDebug(uint infoFlag);
#endif
};

#endif