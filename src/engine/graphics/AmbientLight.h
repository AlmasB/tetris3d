#ifndef __AMBIENT_LIGHT_H__
#define __AMBIENT_LIGHT_H__

#include "Movable.h"
#include "GraphicsEngine.h"

class AmbientLight : public Movable {
	private:
		Point3f center;
		SDL_Colorf color;
		float intensity;

	public:
		AmbientLight(const Point3f & pos, SDL_Colorf color, const float & intensity)
			: center(pos), color(color), intensity(intensity) {}

		void move(const Vector3f & v) { center += v; }
		Point3f getCenter() { return center; }
		SDL_Colorf getColor() { return color; }
		float getIntensity() { return intensity; }

		void setColor(SDL_Colorf _color) { color = _color; }
		void setIntensity(const float & value) { intensity = value; }
};

#endif