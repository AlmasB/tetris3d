#ifndef __GAME_OBJ_H__
#define __GAME_OBJ_H__

#include <GL/glew.h>
#include <GL/gl.h>



#include <memory>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include <iostream>

#include "BoundingBox.h"
#include "Movable.h"
#include "GraphicsEngine.h"

using namespace std;


static const char* vertexShaderCode = "                                             \n\
#version 120                                                                        \n\
								                                                    \n\
attribute vec3 Position;                                                            \n\
attribute vec2 UV;                                                                  \n\
uniform mat4 gWVP;                                                                  \n\
varying vec2 outUV;                                                                 \n\
																					\n\
void main() {                                                                       \n\
    gl_Position = gWVP * vec4(Position, 1.0);                                       \n\
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



// TODO: autocast to float, maybe even macro
struct RGBColor {
	int r, g, b;
};

static const RGBColor COLOR_BLUE = { 0, 0, 255 };
static const RGBColor COLOR_GRAY = { 128, 128, 128 };
static const RGBColor COLOR_RED = { 255, 0, 0 };
static const RGBColor COLOR_GREEN = { 0, 255, 0 };
static const RGBColor COLOR_YELLOW = { 255, 255, 0 };
static const RGBColor COLOR_GOLD = { 255, 215, 0 };
static const RGBColor COLOR_BLACK = { 0, 0, 0 };
static const RGBColor COLOR_AQUA = { 127, 255, 212 };
static const RGBColor COLOR_PURPLE = { 167, 74, 199 };

// BIG TODO: maybe we could subtract half distances of object, so that when we use create obj
// at 0.0.0 it starts drawing from 0.0.0 and not half of it, x-x0 / 2 gives us center etc

class GameObject : public BoundingBox, public Movable {
	private:
	protected:
		RGBColor color;
		uint numOfTriangles;
		
		bool locked;

		CameraTransformer transformer;

		GLuint VBO;
		GLuint EBO;


		GLuint gVAO;
		GLuint gVBO;

		GLuint gWVPLocation;
		GLuint mycolor;
		GLuint useTexture;


		GLuint createBuffer(GLenum, const void *, GLsizei);
		GLuint createShader(const char * shaderCode, GLenum shaderType);
		void compileShaders();

	public:
		GameObject(const Point3f &, float, float, float, RGBColor);
		GameObject(const Point3f &center);	// for dummy objects
		void setDistZ(float);
		void setLocked(bool b);

		void move(const Vector3f &);
		void scale(float, float, float);
		void setScale(float, float, float);
		void setCenter(float, float, float);
		void setCenter(const Point3f & center);
		void rotate(float, float, float);
		void setRotate(float, float, float);
		Point3f getCenter();
		Vector3f getScale();

		void draw();


		GLuint texture;
		GLuint textureID;

		void test();
};

class Cuboid : public GameObject {
	public:
		Cuboid(const Point3f &center, float x, float y, float z, RGBColor);
};

class Cube : public GameObject {
	public:
		Cube(const Point3f &, const float& size, RGBColor);
};

class Plane : public GameObject {
	public:
		Plane(const Point3f &, float, float, float, RGBColor);
		bool collidesWith(const BoundingBox &);
};

#endif
