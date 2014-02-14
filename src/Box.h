#include <SDL_opengl.h>

class Point3 {
	private:
	public:
		double x, y, z;
		
		Point3(double, double, double);
		Point3& operator+=(const Point3 &);
};

class Box {
	private:
	public:
		Point3 center;

		Box(const Point3 &);
		Box(float, float, float);
		void draw();
};