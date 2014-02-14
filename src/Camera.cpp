#include "Camera.h"

Camera::Camera() : position(0, 0, 0), direction(0, 0, -1), worldUP(0, 1, 0), angle(0.0f), speed(0.2f) {
}

void Camera::adjustDirection() {
	direction = Vector3(sin(angle), direction.getY(), -cos(angle));
}

void Camera::moveForward() {
	position += Vector3(direction.getX() * speed, 0, direction.getZ() * speed);
}

void Camera::moveBack() {
	position += Vector3(-direction.getX() * speed, 0, -direction.getZ() * speed);
}

// Z and X are swapped, maybe coz of -cos ?
void Camera::moveRight() {
	position += Vector3(-direction.getZ() * speed, 0, direction.getX() * speed);
}

void Camera::moveLeft() {
	position += Vector3(direction.getZ() * speed, 0, -direction.getX() * speed);
}

// TODO: replace with 1 generic, rotateY or smth

void Camera::rotateUp(float units) {
	direction += Vector3(0, units, 0);
}

void Camera::rotateDown(float units) {
	direction += Vector3(0, -units, 0);
}

void Camera::rotateRight(float units) {
	angle += units;
	adjustDirection();
}

void Camera::rotateLeft(float units) {
	angle += -units;
	adjustDirection();
}

Point3 Camera::getPosition() { return position; }
Vector3 Camera::getDirection() { return direction; }
Vector3 Camera::getUP() { return worldUP; }