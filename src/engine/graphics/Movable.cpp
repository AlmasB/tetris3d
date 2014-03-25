#include "Movable.h"

Movable::Movable() : sensitivity(0.05f), speed(0.15f), direction(Vector3f(0, 0, 1.0f)), up(Vector3f(0, 1.0f, 0)) {
	direction.normalize();

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

void Movable::adjustDirection() {
	const Vector3f vAxis(0.0f, 1.0f, 0.0f);

	// rotate the view vector by the horizontal angle around the vertical axis
	Vector3f view(1.0f, 0.0f, 0.0f);
	view.rotate(horAngle, vAxis);
	view.normalize();

	// rotate the view vector by the vertical angle around the horizontal axis
	Vector3f hAxis = vAxis.cross(view);
	hAxis.normalize();
	view.rotate(verAngle, hAxis);

	direction = view;
	direction.normalize();

	up = direction.cross(hAxis);
	up.normalize();
}

void Movable::move(float x, float y, float z) {
	move(Vector3f(x, y, z));
}

void Movable::moveTo(const Point3f & point) {
	move(point - getCenter());
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

// make sure to have "up" set correctly, if moving horizontally (0,1,0)
void Movable::moveRight(float _speed) {
	Vector3f right = up.cross(direction);
	right.normalize();
	right *= _speed;
	//center += right;
	move(right);
}

void Movable::moveLeft(float _speed) {
	Vector3f left = direction.cross(up);
	left.normalize();
	left *= _speed;
	//center += left;
	move(left);
}

void Movable::lookUp(float degrees) {
	degrees = -degrees;	// to make more natural to use
	verAngle += degrees * sensitivity;
	adjustDirection();
}

void Movable::lookRight(float degrees) {
	horAngle += degrees * sensitivity;
	adjustDirection();
}

void Movable::lookAt(const Point3f &point) {
	direction = point - getCenter();
}

#ifdef __DEBUG
void Movable::printDebug(int infoFlag) {
	if (__CENTER & infoFlag) {
		Point3f p = getCenter();
		std::cout << "DEBUG: CENTER(" << p.x << "," << p.y << "," << p.z << ")" << std::endl;
	}

	if (__DIRECTION & infoFlag) {
		Vector3f p = getDirection();
		std::cout << "DEBUG: DIRECTION(" << p.x << "," << p.y << "," << p.z << ")" << std::endl;
	}

	if (__UP & infoFlag) {
		Vector3f p = getUpVector();
		std::cout << "DEBUG: UP(" << p.x << "," << p.y << "," << p.z << ")" << std::endl;
	}
}
#endif