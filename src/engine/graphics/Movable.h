#ifndef __MOVABLE_H__
#define __MOVABLE_H__

#include "../math/GameMath.h"
#include "../core/Debug.h"

#ifdef __DEBUG
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
		float sensitivity;

		void adjustDirection();

	public:
		Movable();
		virtual void move(const Vector3f & v) = 0;
		void move(float, float, float);
		void moveTo(const Point3f & point);

		/**
		* Allows you to move as if you were looking thru the object's eyes
		* Standard FPS mode
		*/
		void moveForward();
		void moveBackward();
		void moveRight(float speed);
		void moveLeft(float speed);

		/**
		* Allows you to rotate object's view like in FPS
		* For cut scenes use lookAt instead
		*/
		void lookUp(float degrees);
		void lookRight(float degrees);
		void lookAt(const Point3f &point);

		/* ACCESSORS */

		void setSensitivity(float val)			{ sensitivity = val; }
		void setDirection(const Vector3f & dir) { direction = dir; }
		void setUpVector(const Vector3f & _up)  { up = _up; }

		Vector3f getDirection() { return direction; }
		Vector3f getUpVector()  { return up; }
		virtual Point3f getCenter() = 0;

		float getHorAngle() { return horAngle; }
		float getVerAngle() { return verAngle; }

#ifdef __DEBUG
		void printDebug(uint infoFlag);
#endif
};

#endif