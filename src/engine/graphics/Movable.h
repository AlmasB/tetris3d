/**
 * {XCube3D Game Engine}
 * Copyright(C) { 2013-2014 }  {AlmasB} {a.baimagambetov1@uni.brighton.ac.uk}
 *
 * This program is free software; you can redistribute it and / or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110 - 1301 USA.
 */

#ifndef __MOVABLE_H__
#define __MOVABLE_H__

#include "../math/GameMath.h"
#include "../core/Debug.h"

#ifdef __DEBUG
	#define __DIRECTION 1
	#define __CENTER 2
	#define __UP 4
#endif

class Movable {
	protected:
		Vector3f direction;
		Vector3f up;

		float horAngle, verAngle;
		float speed;
		float sensitivity;

		void adjustDirection();

	public:
		Movable();
		virtual void move(const Vector3f & v) = 0;
		void move(float, float, float);
		void moveTo(const Point3f & point);

		/**
		* Allows you to move as if you were looking thru the object's eyes
		* Standard FPS mode
		*/
		void moveForward();
		void moveBackward();
		void moveRight(float speed);
		void moveLeft(float speed);

		/**
		* Allows you to rotate object's view like in FPS
		* For cut scenes use lookAt instead
		*/
		void lookUp(float degrees);
		void lookRight(float degrees);
		void lookAt(const Point3f &point);

		/* ACCESSORS */

		void setSensitivity(float val)			{ sensitivity = val; }
		void setDirection(const Vector3f & dir) { direction = dir; }
		void setUpVector(const Vector3f & _up)  { up = _up; }

		Vector3f getDirection() { return direction; }
		Vector3f getUpVector()  { return up; }
		virtual Point3f getCenter() = 0;

		float getHorAngle() { return horAngle; }
		float getVerAngle() { return verAngle; }

#ifdef __DEBUG
		void printDebug(int infoFlag);
#endif
};

#endif