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

#ifndef __PHYSICS_ENGINE_H__
#define __PHYSICS_ENGINE_H__

#include <vector>
#include <memory>

#include "../math/GameMath.h"

static const float DEFAULT_GRAVITY = -1.0f;

class PhysicsObject;

class PhysicsEngine {
	friend class GameEngine;
	friend class PhysicsObject;
	private:
		Vector3f gravity;
		PhysicsEngine();

		std::vector<std::shared_ptr<PhysicsObject>> objects;

		bool isOnPlatform(std::shared_ptr<PhysicsObject>);

	public:
		/**
		* Note that gravity is naturally a negative value
		* update interval in seconds
		*/
		void setGravity(float gravityValue, float worldUpdateInterval);
		void update();

		void registerObject(std::shared_ptr<PhysicsObject>);
};

class PhysicsObject {
	friend class PhysicsEngine;
	protected:
		Point3f center;
		float lX, lY, lZ, hlX, hlY, hlZ;	// lengths and half lengths

		Vector3f force;

		void applyForce(const Vector3f &);
	public:
		PhysicsObject(const Point3f & center, float x, float y, float z);

		Point3f getCenter() { return center; }
		float getLengthX() { return lX; }
		float getLengthY() { return lY; }
		float getLengthZ() { return lZ; }
		float getHalfLengthX() { return hlX; }
		float getHalfLengthY() { return hlY; }
		float getHalfLengthZ() { return hlZ; }

		bool isColliding(const PhysicsObject & other);
		/**
		* If we have different implementations of engines/gravity
		* this can be very useful
		*/
		virtual void applyGravity(const PhysicsEngine & engine);
		virtual void applyAntiGravity(const PhysicsEngine & engine);
};

#endif