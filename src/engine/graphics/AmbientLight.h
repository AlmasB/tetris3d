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