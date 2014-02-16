#ifndef __GAME_OBJ_H__
#define __GAME_OBJ_H__

#include "BoundingBox.h"

// TODO: autocast to float, maybe even macro
struct RGBColor {
	int r, g, b;
};

static const RGBColor COLOR_BLUE = { 0, 0, 255 };
static const RGBColor COLOR_GRAY = { 128, 128, 128 };
static const RGBColor COLOR_RED = { 255, 0, 0 };
static const RGBColor COLOR_GREEN = { 0, 255, 0 };
static const RGBColor COLOR_YELLOW = { 255, 255, 0 };

class GameObject : public BoundingBox {
	private:
	protected:
		RGBColor color;
		bool locked;
	public:
		GameObject(const Point3 &, float, float, float, RGBColor);
		void move(const Vector3 &);
		void setDistZ(float);
		void setLocked(bool b);
		bool alive;

		//virtual void draw() = 0;	// revisit that
};

class Cube : public GameObject {
	public:
		Cube(const Point3 &, float, RGBColor);
		void draw();
};

class HorizontalPlane : public GameObject {
	public:
		HorizontalPlane(const Point3 &, float, float, float, RGBColor);
		bool collidesWith(const BoundingBox &);
		void draw();
};

typedef HorizontalPlane HPlane;

#endif