#include "BoundingBox.h"

BoundingBox::BoundingBox(const Point3f & _center, float x, float y, float z) 
	: center(_center), 
	halfDistX(Vector3f(x / 2.0f, 0, 0)), _halfDistX(x/2),
	halfDistY(Vector3f(0, y / 2.0f, 0)), _halfDistY(y/2),
	halfDistZ(Vector3f(0, 0, z / 2.0f)), _halfDistZ(z/2) {

}

Point3f BoundingBox::getCenter() const {
	return center;
}

float BoundingBox::getHalfDistY() const {
	return _halfDistY;
}

bool between(const std::pair<float, float> & a, const std::pair<float, float> & b) {
	return (a.first >= b.first && a.first <= b.second) || (a.second >= b.first && a.second <= b.second);
}

inline float projection(const Point3f & pnt, const Vector3f & unitVec) {
	float result;
	result = (pnt.x * unitVec.x);
	result = (result + (pnt.y * unitVec.y));
	result = (result + (pnt.z * unitVec.z));
	return result;
}

bool BoundingBox::collidesWith(const BoundingBox & other) {
	return between(projectOntoAxis(*this, X), projectOntoAxis(other, X)) 
		&& between(projectOntoAxis(*this, Y), projectOntoAxis(other, Y))
		&& between(projectOntoAxis(*this, Z), projectOntoAxis(other, Z));
}

std::pair<float, float> BoundingBox::projectOntoAxis(const BoundingBox & b, enum AXIS axis) {
	float lo, hi;

	switch (axis) {
		case X:
			lo = projection(Point3f(b.center + (halfDistX * -1.0f)), UNIT_X_AXIS);
			hi = projection(Point3f(b.center + halfDistX), UNIT_X_AXIS);
			break;
		case Y:
			lo = projection(Point3f(b.center + (halfDistY * -1.0f)), UNIT_Y_AXIS);
			hi = projection(Point3f(b.center + halfDistY), UNIT_Y_AXIS);
			break;
		case Z:
			lo = projection(Point3f(b.center + (halfDistZ * -1.0f)), UNIT_Z_AXIS);
			hi = projection(Point3f(b.center + halfDistZ), UNIT_Z_AXIS);
			break;
	}

	return std::make_pair(lo, hi);
}

bool BoundingBox::isColliding(const BoundingBox & other) {
	if (center.y + _halfDistY < other.center.y - other._halfDistY ||
		center.y - _halfDistY > other.center.y + other._halfDistY)
		return false;

	Rectf r1 = { center.x - _halfDistX, center.z - _halfDistZ, 2 * _halfDistX, 2 * _halfDistZ };
	Rectf r2 = { other.center.x - other._halfDistX, other.center.z - other._halfDistZ, 2 * other._halfDistX, 2 * other._halfDistZ };

	return r1.intersects(r2);
}




