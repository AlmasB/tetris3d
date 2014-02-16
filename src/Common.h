#ifndef __COMMON_H__
#define __COMMON_H__

#include <math.h>

typedef unsigned int uint;

class Point3;
class Vector3;

class Vector3 {
	private:
		float x, y, z;
	public:
		Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

		inline Vector3& operator += (Vector3 v) {
			this->x += v.getX();
			this->y += v.getY();
			this->z += v.getZ();
			return *this;
		}

		inline Vector3 operator + (Vector3 v) {
			return Vector3(x + v.x, y + v.y, z + v.z);
		}

		inline Vector3 operator * (float scalar) {
			return Vector3(x*scalar, y*scalar, z*scalar);
		}

		inline float getX() const { return x; }
		inline float getY() const { return y; }
		inline float getZ() const { return z; }
};

class Point3 {
	private:
		float x, y, z;
	public:
		Point3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
		
		inline Point3 operator + (Vector3 v) const {
			return Point3(this->x + v.getX(), this->y + v.getY(), this->z + v.getZ());
		}

		inline Point3& operator += (Vector3 v) {
			this->x += v.getX();
			this->y += v.getY();
			this->z += v.getZ();
			return *this;
		}

		inline float getX() const { return x; }
		inline float getY() const { return y; }
		inline float getZ() const { return z; }
};

inline float distanceBetween(const Point3 & p1, const Point3 & p2) {
	float dx = p2.getX() - p1.getX();
	float dy = p2.getY() - p1.getY();
	float dz = p2.getZ() - p1.getZ();

	return sqrt(dx*dx + dy*dy + dz*dz);
}

struct Point2 {
	int x, y;
};

struct Rectangle2 {
	int x, y, w, h;

	inline bool contains(const Point2 & p) {
		return p.x >= x && p.x <= x + w
			&& p.y >= y && p.y <= y + h;
	}
};

typedef Rectangle2 Rect;






#endif
