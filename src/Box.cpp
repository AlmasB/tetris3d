#include "Box.h"


Box::Box(const Point3 & _center) : center(_center) {

}

Box::Box(float x, float y, float z) : center(Point3(x, y, z)) {
}

void Box::draw() {
	glPushMatrix();
	glTranslatef(center.getX(), center.getY(), center.getZ());  // Move right and into the screen

	glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
	// Top face (y = 1.0f)
	// Define vertices in counter-clockwise (CCW) order with normal pointing out
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// Bottom face (y = -1.0f)
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Front face  (z = 1.0f)
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Back face (z = -1.0f)
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	// Left face (x = -1.0f)
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Right face (x = 1.0f)
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();

	glPopMatrix();
}

bool between(const std::pair<float, float> & a, const std::pair<float, float> & b) {
	return (a.first >= b.first && a.first <= b.second) || (a.second >= b.first && a.second <= b.second);
}

inline float projection(const Point3 & pnt, const Vector3 & unitVec)
{
	float result;
	result = (pnt.getX() * unitVec.getX());
	result = (result + (pnt.getY() * unitVec.getY()));
	result = (result + (pnt.getZ() * unitVec.getZ()));
	return result;
}

bool Box::collidesWith(const Box & other) {
	return between(projectOntoAxis(*this, X), projectOntoAxis(other, X)) 
		&& between(projectOntoAxis(*this, Y), projectOntoAxis(other, Y))
		&& between(projectOntoAxis(*this, Z), projectOntoAxis(other, Z));
}

std::pair<float, float> Box::projectOntoAxis(const Box & b, enum AXIS axis) {

	float lo, hi;

	Vector3 h_x(1.0, 0, 0);
	Vector3 h_y(0, 1.0, 0);
	Vector3 h_z(0, 0, 1.0);

	switch (axis) {
	case X:
		lo = projection(Point3(b.center + (h_x * -1.0f)), UNIT_X_AXIS);
		hi = projection(Point3(b.center + h_x), UNIT_X_AXIS);
		break;
	case Y:
		lo = projection(Point3(b.center + (h_y * -1.0f)), UNIT_Y_AXIS);
		hi = projection(Point3(b.center + h_y), UNIT_Y_AXIS);
		break;
	case Z:
		lo = projection(Point3(b.center + (h_z * -1.0f)), UNIT_Z_AXIS);
		hi = projection(Point3(b.center + h_z), UNIT_Z_AXIS);
		break;
	}

	return std::make_pair(lo, hi);
}




