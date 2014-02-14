#include <SDL_opengl.h>

#include "Common.h"

class Box {
	private:
	public:
		Point3 center;

		Box(const Point3 &);
		Box(float, float, float);
		void draw();
};