/*
	This is a compilation of several math libraries, as well as my own

	Original sources can be found here:
	Copyright 2010, Etay Meiri - http://ogldev.atspace.co.uk/index.html
	Copyrigth 2006, 2007 Sony Computer Entertainment Inc. - https://github.com/AidanDelaney/CI224-game/blob/master/include/vectormath/scalar/cpp/vectormath_aos.h*/

/*	Copyright 2010 Etay Meiri

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

/*	Copyright (C) 2006, 2007 Sony Computer Entertainment Inc.
	All rights reserved.

	Redistribution and use in source and binary forms,
	with or without modification, are permitted provided that the
	following conditions are met:
	* Redistributions of source code must retain the above copyright
	notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	notice, this list of conditions and the following disclaimer in the
	documentation and/or other materials provided with the distribution.
	* Neither the name of the Sony Computer Entertainment Inc nor the names
	of its contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
	ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
	LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
	ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __GAME_MATH_H__
#define __GAME_MATH_H__

#ifdef _WIN32
//#define _USE_MATH_DEFINES 
#include <cmath>
#else
#include <math.h>
#endif

#include <cstdlib>

#define __PI       3.14159265358979323846
#define toRadian(x) (float)(((x) * __PI / 180.0f))
#define toDegree(x) (float)(((x) * 180.0f / __PI))

typedef unsigned int uint;

struct Quaternion {
	float x, y, z, w;

	Quaternion(float _x, float _y, float _z, float _w) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	void normalize() {
		float length = sqrtf(x * x + y * y + z * z + w * w);

		x /= length;
		y /= length;
		z /= length;
		w /= length;
	}

	Quaternion conjugate() {
		Quaternion ret(-x, -y, -z, w);
		return ret;
	}

	Quaternion mult(const Quaternion& r) {
		const float _w = (w * r.w) - (x * r.x) - (y * r.y) - (z * r.z);
		const float _x = (x * r.w) + (w * r.x) + (y * r.z) - (z * r.y);
		const float _y = (y * r.w) + (w * r.y) + (z * r.x) - (x * r.z);
		const float _z = (z * r.w) + (w * r.z) + (x * r.y) - (y * r.x);

		Quaternion ret(_x, _y, _z, _w);

		return ret;
	}
};

/*Quaternion operator*(const Quaternion& l, const Quaternion& r) {
	const float w = (l.w * r.w) - (l.x * r.x) - (l.y * r.y) - (l.z * r.z);
	const float x = (l.x * r.w) + (l.w * r.x) + (l.y * r.z) - (l.z * r.y);
	const float y = (l.y * r.w) + (l.w * r.y) + (l.z * r.x) - (l.x * r.z);
	const float z = (l.z * r.w) + (l.w * r.z) + (l.x * r.y) - (l.y * r.x);

	Quaternion ret(x, y, z, w);

	return ret;
}*/

struct Vector3f {
	float x;
	float y;
	float z;

	Vector3f() {
		// need ?
	}

	Vector3f(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f& operator+=(const Vector3f& r) {
		x += r.x;
		y += r.y;
		z += r.z;

		return *this;
	}

	Vector3f& operator-=(const Vector3f& r) {
		x -= r.x;
		y -= r.y;
		z -= r.z;

		return *this;
	}

	Vector3f& operator*=(float f) {
		x *= f;
		y *= f;
		z *= f;

		return *this;
	}

	Vector3f cross(const Vector3f& v) const {
		const float _x = y * v.z - z * v.y;
		const float _y = z * v.x - x * v.z;
		const float _z = x * v.y - y * v.x;

		return Vector3f(_x, _y, _z);
	}

	Vector3f& normalize() {
		const float length = sqrtf(x * x + y * y + z * z);

		x /= length;
		y /= length;
		z /= length;

		return *this;
	}

	void rotate(float angle, const Vector3f& axis) {
		const float sinHalfAngle = sinf(toRadian(angle/2));
    	const float cosHalfAngle = cosf(toRadian(angle/2));

    	const float Rx = axis.x * sinHalfAngle;
    	const float Ry = axis.y * sinHalfAngle;
    	const float Rz = axis.z * sinHalfAngle;
    	const float Rw = cosHalfAngle;
    	Quaternion RotationQ(Rx, Ry, Rz, Rw);

   	 	Quaternion ConjugateQ = RotationQ.conjugate();
 	    //  ConjugateQ.Normalize();
    	//Quaternion W = RotationQ * (*this) * ConjugateQ;
		//Quaternion W = this->multQuaternion(RotationQ) * ConjugateQ;
		Quaternion W = this->multQuaternion(RotationQ).mult(ConjugateQ);

    	x = W.x;
    	y = W.y;
   	 	z = W.z;
	}

	Quaternion multQuaternion(const Quaternion& q) {
		const float _w = -(q.x * x) - (q.y * y) - (q.z * z);
		const float _x = (q.w * x) + (q.y * z) - (q.z * y);
		const float _y = (q.w * y) + (q.z * x) - (q.x * z);
		const float _z = (q.w * z) + (q.x * y) - (q.y * x);

		Quaternion ret(_x, _y, _z, _w);

		return ret;
	}
};


/*Quaternion operator*(const Quaternion& q, const Vector3f& v) {
	const float w = -(q.x * v.x) - (q.y * v.y) - (q.z * v.z);
	const float x = (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
	const float y = (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
	const float z = (q.w * v.z) + (q.x * v.y) - (q.y * v.x);

	Quaternion ret(x, y, z, w);

	return ret;
}*/

inline Vector3f operator+(const Vector3f& l, const Vector3f& r) {
	Vector3f Ret(l.x + r.x, l.y + r.y, l.z + r.z);
	return Ret;
}

inline Vector3f operator-(const Vector3f& l, const Vector3f& r) {
	Vector3f Ret(l.x - r.x, l.y - r.y, l.z - r.z);
	return Ret;
}

inline Vector3f operator*(const Vector3f& l, float f) {
	Vector3f Ret(l.x * f, l.y * f, l.z * f);
	return Ret;
}

class Matrix4f {
public:
	float m[4][4];

	Matrix4f() {	// init array with smth ?
	}


	inline void initIdentity()
	{
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	}

	inline Matrix4f operator*(const Matrix4f& right) const
	{
		Matrix4f Ret;

		for (unsigned int i = 0; i < 4; i++) {
			for (unsigned int j = 0; j < 4; j++) {
				Ret.m[i][j] = m[i][0] * right.m[0][j] +
					m[i][1] * right.m[1][j] +
					m[i][2] * right.m[2][j] +
					m[i][3] * right.m[3][j];
			}
		}

		return Ret;
	}

	void initScaleTransform(float ScaleX, float ScaleY, float ScaleZ) {
		m[0][0] = ScaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
		m[1][0] = 0.0f;   m[1][1] = ScaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
		m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = ScaleZ; m[2][3] = 0.0f;
		m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
	}

	void initRotateTransform(float RotateX, float RotateY, float RotateZ) {
		Matrix4f rx, ry, rz;

		const float x = toRadian(RotateX);
		const float y = toRadian(RotateY);
		const float z = toRadian(RotateZ);

		rx.m[0][0] = 1.0f; rx.m[0][1] = 0.0f; rx.m[0][2] = 0.0f; rx.m[0][3] = 0.0f;
		rx.m[1][0] = 0.0f; rx.m[1][1] = cosf(x); rx.m[1][2] = -sinf(x); rx.m[1][3] = 0.0f;
		rx.m[2][0] = 0.0f; rx.m[2][1] = sinf(x); rx.m[2][2] = cosf(x); rx.m[2][3] = 0.0f;
		rx.m[3][0] = 0.0f; rx.m[3][1] = 0.0f; rx.m[3][2] = 0.0f; rx.m[3][3] = 1.0f;

		ry.m[0][0] = cosf(y); ry.m[0][1] = 0.0f; ry.m[0][2] = -sinf(y); ry.m[0][3] = 0.0f;
		ry.m[1][0] = 0.0f; ry.m[1][1] = 1.0f; ry.m[1][2] = 0.0f; ry.m[1][3] = 0.0f;
		ry.m[2][0] = sinf(y); ry.m[2][1] = 0.0f; ry.m[2][2] = cosf(y); ry.m[2][3] = 0.0f;
		ry.m[3][0] = 0.0f; ry.m[3][1] = 0.0f; ry.m[3][2] = 0.0f; ry.m[3][3] = 1.0f;

		rz.m[0][0] = cosf(z); rz.m[0][1] = -sinf(z); rz.m[0][2] = 0.0f; rz.m[0][3] = 0.0f;
		rz.m[1][0] = sinf(z); rz.m[1][1] = cosf(z); rz.m[1][2] = 0.0f; rz.m[1][3] = 0.0f;
		rz.m[2][0] = 0.0f; rz.m[2][1] = 0.0f; rz.m[2][2] = 1.0f; rz.m[2][3] = 0.0f;
		rz.m[3][0] = 0.0f; rz.m[3][1] = 0.0f; rz.m[3][2] = 0.0f; rz.m[3][3] = 1.0f;

		*this = rz * ry * rx;
	}

	void initTranslationTransform(float x, float y, float z) {
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	}

	void initCameraTransform(const Vector3f& Target, const Vector3f& Up) {
		Vector3f N = Target;
		N.normalize();
		Vector3f U = Up;
		U.normalize();
		U = U.cross(N);
		Vector3f V = N.cross(U);

		m[0][0] = U.x;   m[0][1] = U.y;   m[0][2] = U.z;   m[0][3] = 0.0f;
		m[1][0] = V.x;   m[1][1] = V.y;   m[1][2] = V.z;   m[1][3] = 0.0f;
		m[2][0] = N.x;   m[2][1] = N.y;   m[2][2] = N.z;   m[2][3] = 0.0f;
		m[3][0] = 0.0f;  m[3][1] = 0.0f;  m[3][2] = 0.0f;  m[3][3] = 1.0f;
	}

	void initPersProjTransform(float FOV, float Width, float Height, float zNear, float zFar) {
		const float ar = Width / Height;
		const float zRange = zNear - zFar;
		const float tanHalfFOV = tanf(toRadian(FOV / 2.0f));

		m[0][0] = 1.0f / (tanHalfFOV * ar); m[0][1] = 0.0f;            m[0][2] = 0.0f;          m[0][3] = 0.0;
		m[1][0] = 0.0f;                   m[1][1] = 1.0f / tanHalfFOV; m[1][2] = 0.0f;          m[1][3] = 0.0;
		m[2][0] = 0.0f;                   m[2][1] = 0.0f;            m[2][2] = (-zNear - zFar) / zRange; m[2][3] = 2.0f * zFar*zNear / zRange;
		m[3][0] = 0.0f;                   m[3][1] = 0.0f;            m[3][2] = 1.0f;          m[3][3] = 0.0;
	}
};

struct Point3f {
	float x, y, z ;

	Point3f(float _x, float _y, float _z) {
		x = _x; y = _y; z = _z;
	}

	inline Point3f& operator += (Vector3f v) {
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		return *this;
	}
};

inline Point3f operator + (Point3f p, Vector3f v) {
	return Point3f(p.x + v.x, p.y + v.y, p.z + v.z);
}

inline float distanceBetween(const Point3f & p1, const Point3f & p2) {
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;
	float dz = p2.z - p1.z;

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

/**
* @return
*			a random integer value between "min" and "max", both inclusive
*/
inline int getRandom(int min, int max) {	// atm no need for random seed
	return (int)(rand() % (max - min)) + min;
}




#endif
