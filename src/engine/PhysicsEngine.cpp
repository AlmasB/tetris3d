#include "PhysicsEngine.h"

PhysicsObject::PhysicsObject(const Point3f & center, float x, float y, float z)
: center(center), lX(x), lY(y), lZ(z), hlX(x / 2.0f), hlY(y / 2.0f), hlZ(z / 2.0f) {}

bool PhysicsObject::isColliding(const PhysicsObject & other) {
	if (center.y + hlY < other.center.y - other.hlY ||
		center.y - hlY > other.center.y + other.hlY)
		return false;

	Rectf r1 = { center.x - hlX, center.z - hlZ, lX, lZ };
	Rectf r2 = { other.center.x - other.hlX, other.center.z - other.hlZ, other.lX, other.lZ };

	return r1.intersects(r2);
}