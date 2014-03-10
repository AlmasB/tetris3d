#include "GameObject.h"

GameObject::GameObject(const Point3f & center, float x, float y, float z, SDL_Color color)
: Cuboid(center, x, y, z, color), locked(false) {}

GameObject::GameObject(const Point3f & center, float x, float y, float z, GLuint textureID)
: Cuboid(center, x, y, z, textureID), locked(false) {}

GameObject::GameObject(const Point3f & center, float size, SDL_Color color)
: GameObject(center, size, size, size, color) {}

GameObject::GameObject(const Point3f & center, float size, GLuint textureID)
: GameObject(center, size, size, size, textureID) {}

void GameObject::setLocked(bool b) {
	if (locked = b)	// NOT typo! (locked = b)
		setColor(SDL_COLOR_YELLOW);
	else
		resetColor();
}



