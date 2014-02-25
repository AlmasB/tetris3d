#include "GameObject.h"

GameObject::GameObject(const Point3f &_center, float x, float y, float z, SDL_Color _color)
: Cuboid(_center, x, y, z, _color) {

}

GameObject::GameObject(const Point3f &_center, float x, float y, float z, GLuint _textureID)
: Cuboid(_center, x, y, z, _textureID) {

}

void GameObject::setLocked(bool b) {
	locked = b;
	if (locked) {
		setColor(SDL_COLOR_YELLOW);
	}
	else {
		resetColor();
	}
}



