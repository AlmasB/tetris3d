#include "BoundingBox.h"

BoundingBox::BoundingBox(const Point3 & _center, float x, float y, float z) 
	: center(_center), 
	halfDistX(Vector3(x / 2.0f, 0, 0)), 
	halfDistY(Vector3(0, y / 2.0f, 0)), 
	halfDistZ(Vector3(0, 0, z / 2.0f)) {

}

Point3 BoundingBox::getCenter() {
	return center;
}

bool between(const std::pair<float, float> & a, const std::pair<float, float> & b) {
	return (a.first >= b.first && a.first <= b.second) || (a.second >= b.first && a.second <= b.second);
}

inline float projection(const Point3 & pnt, const Vector3 & unitVec) {
	float result;
	result = (pnt.getX() * unitVec.getX());
	result = (result + (pnt.getY() * unitVec.getY()));
	result = (result + (pnt.getZ() * unitVec.getZ()));
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
			lo = projection(Point3(b.center + (halfDistX * -1.0f)), UNIT_X_AXIS);
			hi = projection(Point3(b.center + halfDistX), UNIT_X_AXIS);
			break;
		case Y:
			lo = projection(Point3(b.center + (halfDistY * -1.0f)), UNIT_Y_AXIS);
			hi = projection(Point3(b.center + halfDistY), UNIT_Y_AXIS);
			break;
		case Z:
			lo = projection(Point3(b.center + (halfDistZ * -1.0f)), UNIT_Z_AXIS);
			hi = projection(Point3(b.center + halfDistZ), UNIT_Z_AXIS);
			break;
	}

	return std::make_pair(lo, hi);
}




