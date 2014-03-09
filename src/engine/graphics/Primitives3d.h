#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__

#include "../physics/PhysicsEngine.h"
#include "Movable.h"
#include "GraphicsEngine.h"

#include "ShaderManager.h"

static const char* vertexShaderCode = "                                             \n\
#version 120                                                                        \n\
																					\n\
attribute vec3 position;                                                            \n\
attribute vec2 UV;                                                                  \n\
uniform mat4 mvp;                                                                   \n\
varying vec2 outUV;                                                                 \n\
																				    \n\
uniform int useUI;																	\n\
																					\n\
void main() {                                                                       \n\
	if (useUI == 0) {																\n\
		gl_Position = mvp * vec4(position, 1.0);                                    \n\
	}else{																			\n\
		vec2 size = position.xy - vec2(400, 300);									\n\
		size /= vec2(400, 300);														\n\
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
																					\n\
void main() {                                                                       \n\
	gl_FragColor = useTexture > 0 ? texture2D(sampler, outUV) : color;  	        \n\
}";

// BIG TODO: maybe we could subtract half distances of object, so that when we use create obj
// at 0.0.0 it starts drawing from 0.0.0 and not half of it, x-x0 / 2 gives us center etc

class Primitive3d : public PhysicsObject, public Movable {
	protected:
		GLuint shaderProgram;
		CameraTransformer transformer;

		GLuint vbo, ibo;	// vertices, indices

		SDL_Color originalColor;	// TODO: color/texture might not be used at this level of abstraction
		SDL_Color color;			// besides draw() = 0
		GLuint colorLocation;

		GLuint textureID;
		GLuint textureIDLocation;
		GLuint useTextureLocation;	// we pass 1 to use texture and 0 to use color

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

#endif