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

#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__

#include "../physics/PhysicsEngine.h"
#include "AmbientLight.h"

#include "ShaderManager.h"

#include <vector>

// TODO: in later versions create external glsl files

static const char* vertexShaderCode = "                                             \n\
#version 120                                                                        \n\
																					\n\
attribute vec3 Position;                                                            \n\
attribute vec2 UV;                                                                  \n\
uniform mat4 mvp;                                                                   \n\
varying vec2 outUV;                                                                 \n\
uniform vec2 windowSize;														    \n\
																				    \n\
uniform int useUI;																	\n\
																					\n\
void main() {                                                                       \n\
	if (useUI == 0) {																\n\
		gl_Position = mvp * vec4(Position, 1.0);                                    \n\
	}else{																			\n\
		vec2 size = Position.xy - windowSize;										\n\
		size /= windowSize;															\n\
		gl_Position = vec4(size, 0, 1);												\n\
	}																				\n\
																					\n\
	outUV = UV;																		\n\
}";

static const char* fragmentShaderCode = "                                           \n\
#version 120                                                                        \n\
varying vec2 outUV;						                                            \n\
uniform vec4 color;                                                                 \n\
uniform sampler2D sampler;														    \n\
uniform int useTexture;                                                             \n\
struct DirectionalLight {                                                           \n\
    vec3 color;                                                                     \n\
	float ambientIntensity;                                                         \n\
};                                                                                  \n\
uniform DirectionalLight light;                                                     \n\
																					\n\
void main() {                                                                       \n\
	//gl_FragColor = useTexture > 0 ? texture2D(sampler, outUV) : color;  	        \n\
	gl_FragColor = (useTexture > 0 ? texture2D(sampler, outUV) : color)             \n\
		* vec4(light.color, 1.0) * light.ambientIntensity;  	                    \n\
}";


// ambient intensity increases effect of that color the greater the value the greater effect

// BIG TODO: maybe we could subtract half distances of object, so that when we use create obj
// at 0.0.0 it starts drawing from 0.0.0 and not half of it, x-x0 / 2 gives us center etc
// where do we want to "actually" draw the ground line 0,0,0 ?
// then change values there GameObject 0,1,0 and 2.0f makes sense a bit more then 0,0,0, 2.0f

class Primitive3d : public PhysicsObject, public Movable {
	protected:
		GLuint shaderProgram;
		CameraTransformer transformer;

		GLuint vbo, ibo;	// vertices, indices

		SDL_Colorf originalColor;	// TODO: color/texture might not be used at this level of abstraction
		SDL_Colorf color;			// besides draw() = 0
		GLuint colorLocation;

		GLuint textureID;
		GLuint textureIDLocation;
		GLuint useTextureLocation;	// we pass 1 to use texture and 0 to use color

		SDL_Colorf ambientLightColor;
		float ambientLightIntensity;

		GLuint lightColorLocation;	// light struct gpu memory location
		GLuint lightIntensityLocation;

		GLuint mvpLocation;	// model view projection
		GLuint useUI;	// TODO: rename

		GLuint createBuffer(GLenum, const void *, GLsizei);

	public:
		Primitive3d(const Point3f &center, float lengthX, float lengthY, float lengthZ, SDL_Color color);
		Primitive3d(const Point3f &center, float lengthX, float lengthY, float lengthZ, GLuint textureID);
		Primitive3d(const Point3f &center, float lengthX, float lengthY, float lengthZ, SDL_Color color, GLuint textureID);

		void setColor(SDL_Color color);
		void resetColor();
		void setTexture(GLuint texture);

		void move(const Vector3f &);
		void scale(float, float, float);
		void setScale(float, float, float);
		void setCenter(float, float, float);
		void setCenter(const Point3f & center);
		void rotate(float, float, float);
		void setRotate(float, float, float);
		Point3f getCenter();
		Vector3f getScale();

		void updateCameraTransforms();

		virtual void applyGravity(const PhysicsEngine & engine);
		virtual void applyAntiGravity(const PhysicsEngine & engine);

		void moveWithPhysics(const Vector3f &);

		void setAmbientLightColor(SDL_Colorf, float intensity);

		virtual void draw() = 0;
};

class Cuboid : public Primitive3d {
	public:
		Cuboid(const Point3f & center, float x, float y, float z, SDL_Color color);
		Cuboid(const Point3f & center, float x, float y, float z, GLuint textureID);
		Cuboid(const Point3f & center, float lX, float lY, float lZ, SDL_Color color, GLuint textureID);
		virtual void draw();
};

class Cube : public Cuboid {
	public:
		Cube(const Point3f & center, float size, SDL_Color color);
		Cube(const Point3f & center, float size, GLuint textureID);
};

class Sphere : public Primitive3d {
	private:
		int slices, stacks;
	public:
		Sphere(const Point3f & center, const float & size, SDL_Color color);
		// ST coordinates are not mapped correctly at the moment
		Sphere(const Point3f & center, const float & size, GLuint textureID);
		virtual void draw();
};

#endif