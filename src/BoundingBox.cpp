#include "BoundingBox.h"

BoundingBox::BoundingBox(const Point3f & _center, float x, float y, float z) 
	: center(_center), 
	halfDistX(Vector3f(x / 2.0f, 0, 0)), 
	halfDistY(Vector3f(0, y / 2.0f, 0)), 
	halfDistZ(Vector3f(0, 0, z / 2.0f)) {

}

Point3f BoundingBox::getCenter() {
	return center;
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




