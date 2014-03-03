#ifndef __MOVABLE_H__
#define __MOVABLE_H__

#include "GameMath.h"
#include "Debug.h"

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

		void setSensitivity(float val);
		void setDirection(const Vector3f & dir);
		void setUpVector(const Vector3f & up);

		Vector3f getDirection();
		Vector3f getUpVector();
		virtual Point3f getCenter() = 0;

		float getHorAngle();
		float getVerAngle();

#ifdef __DEBUG
		void printDebug(uint infoFlag);
#endif
};

#endif