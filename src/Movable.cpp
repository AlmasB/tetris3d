#include "Movable.h"

Movable::Movable() {
	speed = 0.15f;

	direction = Vector3f(0.0f, 0.0f, 1.0f);
	direction.normalize();
	up = Vector3f(0.0f, 1.0f, 0.0f);

	Vector3f horDirection(direction.x, 0.0, direction.z);
	horDirection.normalize();

	if (horDirection.z >= 0.0f) {
		horAngle = horDirection.x >= 0.0f ? 360.0f - toDegree(asin(horDirection.z)) : 180.0f + toDegree(asin(horDirection.z));
	}
	else {
		horAngle = horDirection.x >= 0.0f ? toDegree(asin(-horDirection.z)) : 90.0f + toDegree(asin(-horDirection.z));
	}

	verAngle = -toDegree(asin(direction.y));
}


Vector3f Movable::getDirection() {
	return direction;
}

Vector3f Movable::getUpVector() {
	return up;
}

void Movable::adjustDirection() {
	const Vector3f vaxis(0.0f, 1.0f, 0.0f);

	// Rotate the view vector by the horizontal angle around the vertical axis
	Vector3f view(1.0f, 0.0f, 0.0f);
	view.rotate(horAngle, vaxis);
	view.normalize();

	// Rotate the view vector by the vertical angle around the horizontal axis
	Vector3f hAxis = vaxis.cross(view);
	hAxis.normalize();
	view.rotate(verAngle, hAxis);

	direction = view;
	direction.normalize();

	up = direction.cross(hAxis);
	up.normalize();
}

void Movable::moveForward() {
	//center += (direction * speed);	// for moving in Y as well
	Vector3f tmp = direction * speed;
	tmp.y = 0.0f;
	//center += tmp;
	move(tmp);
}

void Movable::moveBackward() {
	//center -= (direction * speed);
	Vector3f tmp = direction * speed;
	tmp.y = 0.0f;
	//center -= tmp;
	move(tmp * (-1.0f));
}

void Movable::moveRight() {
	Vector3f right = up.cross(direction);
	right.normalize();
	right *= speed;
	//center += right;
	move(right);
}

void Movable::moveLeft() {
	Vector3f left = direction.cross(up);
	left.normalize();
	left *= speed;
	//center += left;
	move(left);
}

void Movable::lookUp(float units) {
	verAngle += units;
	adjustDirection();
}

void Movable::lookRight(float units) {
	horAngle += units;
	adjustDirection();
}