#ifndef __COMMON_H__
#define __COMMON_H__

typedef unsigned int uint;

class Point3;
class Vector3;

class Vector3 {
	private:
		float x, y, z;
	public:
		Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

		inline Vector3& operator += (const Vector3 & v) {
			this->x += v.getX();
			this->y += v.getY();
			this->z += v.getZ();
			return *this;
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
		
		inline Point3 operator + (const Vector3 & v) {
			return Point3(this->x + v.getX(), this->y + v.getY(), this->z + v.getZ());
		}

		inline Point3& operator += (const Vector3 & v) {
			this->x += v.getX();
			this->y += v.getY();
			this->z += v.getZ();
			return *this;
		}

		inline float getX() const { return x; }
		inline float getY() const { return y; }
		inline float getZ() const { return z; }
};








#endif