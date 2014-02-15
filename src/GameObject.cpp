#include "GameObject.h"

GameObject::GameObject(const Point3 & _center, float x, float y, float z, RGBColor _color)
	: BoundingBox(_center, x, y, z), color(_color) {

}



Cube::Cube(const Point3 & _center, float size, RGBColor _color) 
	: GameObject(_center, size, size, size, _color) {

}

void Cube::draw() {
	glPushMatrix();
	glTranslatef(center.getX(), center.getY(), center.getZ());

	glColor3f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);

	glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
	// Top face (y = 1.0f)
	// Define vertices in counter-clockwise (CCW) order with normal pointing out
	glVertex3f(halfDistX.getX(), halfDistY.getY(), -halfDistZ.getZ());
	glVertex3f(-halfDistX.getX(), halfDistY.getY(), -halfDistZ.getZ());
	glVertex3f(-halfDistX.getX(), halfDistY.getY(), halfDistZ.getZ());
	glVertex3f(halfDistX.getX(), halfDistY.getY(), halfDistZ.getZ());

	// Bottom face (y = -1.0f)
	glVertex3f(halfDistX.getX(), -halfDistY.getY(), halfDistZ.getZ());
	glVertex3f(-halfDistX.getX(), -halfDistY.getY(), halfDistZ.getZ());
	glVertex3f(-halfDistX.getX(), -halfDistY.getY(), -halfDistZ.getZ());
	glVertex3f(halfDistX.getX(), -halfDistY.getY(), -halfDistZ.getZ());

	// Front face  (z = 1.0f)
	glVertex3f(halfDistX.getX(), halfDistY.getY(), halfDistZ.getZ());
	glVertex3f(-halfDistX.getX(), halfDistY.getY(), halfDistZ.getZ());
	glVertex3f(-halfDistX.getX(), -halfDistY.getY(), halfDistZ.getZ());
	glVertex3f(halfDistX.getX(), -halfDistY.getY(), halfDistZ.getZ());

	// Back face (z = -1.0f)
	glVertex3f(halfDistX.getX(), -halfDistY.getY(), -halfDistZ.getZ());
	glVertex3f(-halfDistX.getX(), -halfDistY.getY(), -halfDistZ.getZ());
	glVertex3f(-halfDistX.getX(), halfDistY.getY(), -halfDistZ.getZ());
	glVertex3f(halfDistX.getX(), halfDistY.getY(), -halfDistZ.getZ());

	// Left face (x = -1.0f)
	glVertex3f(-halfDistX.getX(), halfDistY.getY(), halfDistZ.getZ());
	glVertex3f(-halfDistX.getX(), halfDistY.getY(), -halfDistZ.getZ());
	glVertex3f(-halfDistX.getX(), -halfDistY.getY(), -halfDistZ.getZ());
	glVertex3f(-halfDistX.getX(), -halfDistY.getY(), halfDistZ.getZ());

	// Right face (x = 1.0f)
	glVertex3f(halfDistX.getX(), halfDistY.getY(), -halfDistZ.getZ());
	glVertex3f(halfDistX.getX(), halfDistY.getY(), halfDistZ.getZ());
	glVertex3f(halfDistX.getX(), -halfDistY.getY(), halfDistZ.getZ());
	glVertex3f(halfDistX.getX(), -halfDistY.getY(), -halfDistZ.getZ());
	glEnd();

	glPopMatrix();
}

HorizontalPlane::HorizontalPlane(const Point3 & c, float x, float y, float z, RGBColor _color)
: GameObject(c, x, y, z, _color) {

}

void HorizontalPlane::draw() {
	glPushMatrix();
	glTranslatef(center.getX(), center.getY(), center.getZ());

	glColor3f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);

	glBegin(GL_QUADS);
	glVertex3f(-halfDistX.getX(), 0.0f, -halfDistZ.getZ());	// forget about Y for now
	glVertex3f(-halfDistX.getX(), 0.0f, halfDistZ.getZ());
	glVertex3f(halfDistX.getX(), 0.0f, halfDistZ.getZ());
	glVertex3f(halfDistX.getX(), 0.0f, -halfDistZ.getZ());
	glEnd();

	glPopMatrix();
}

bool HorizontalPlane::collidesWith(const BoundingBox & box) {
	Rect plane = { (int)-halfDistX.getX(), (int)-halfDistZ.getZ(), 2 * (int)halfDistX.getX(), 2 * (int)halfDistZ.getZ() };
	Point2 p = { (int)box.center.getX(), (int)box.center.getY() };

	return center.getY() > box.center.getY() - box.halfDistY.getY() - 0.05
		&& center.getY() < box.center.getY() - box.halfDistY.getY() + 0.05
		&& plane.contains(p);

	/*return center.getY() == box.center.getY() + box.halfDistY.getY() 
		|| center.getY() == box.center.getY() - box.halfDistY.getY();*/
}