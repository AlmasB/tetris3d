#ifndef __MOVABLE_H__
#define __MOVABLE_H__

#include "GameMath.h"

class Movable {

protected:
	//Vector3f direction;
	Vector3f up;

	float horAngle, verAngle;
	float speed;

public:
	Movable();
	//Movable(const Point3f & center);
	virtual void move(const Vector3f & v) = 0;
	// if you were looking thru the object's eyes
	void moveForward();
	void moveBackward();
	void moveRight();
	void moveLeft();

	void lookUp(float);
	void lookRight(float);
	void adjustDirection();

	Vector3f getDirection();
	Vector3f getUpVector();
	virtual Point3f getCenter() = 0;

	Vector3f direction;
};

#endif