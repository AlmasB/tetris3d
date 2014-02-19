#ifndef __GAME_OBJ_H__
#define __GAME_OBJ_H__

#include <GL/glew.h>
#include <GL/gl.h>

#include "BoundingBox.h"

#include <memory>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include <iostream>


#include "Pipeline.h"
#include "Camera.h"

using namespace std;


static const char* pVS = "                                                          \n\
#version 120                                                                        \n\
								                                                    \n\
attribute vec3 Position;                                                            \n\
                                                                                    \n\
uniform mat4 gWVP;                                                                  \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWVP * vec4(Position, 1.0);                                       \n\
}";



static const char* pFS = "                                                          \n\
#version 120                                                                        \n\
						                                                            \n\
uniform vec4 color;                                                                 \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
	gl_FragColor = color;  					                                        \n\
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

class GameObject : public BoundingBox {
	private:
	protected:
		
		bool locked;

		GLuint VBO;
		GLuint EBO;
		GLuint gWVPLocation;

		GLuint mycolor;

		

		int numOfTriangles;

		GLuint createBuffer(GLenum, const void *, GLsizei);
		GLuint createShader(const char * shaderCode, GLenum shaderType);
		void compileShaders();

	public:
		GameObject(const Point3f &, float, float, float, RGBColor);
		void move(const Vector3f &);
		void setDistZ(float);
		void setLocked(bool b);
		bool alive;

		RGBColor color;

		CameraTransformer transformer;

		void draw();

		void scale(float, float, float);
		void setCenter(float, float, float);
		void rotate(float, float, float);


		//virtual void draw() = 0;	// revisit that
};

class Cube : public GameObject {
	public:
		Cube(const Point3f &, RGBColor);
		//void draw(std::shared_ptr<Camera>);
		
};

class HorizontalPlane : public GameObject {
	public:
		HorizontalPlane(const Point3f &, float, float, float, RGBColor);
		bool collidesWith(const BoundingBox &);
		//void draw();
};

typedef HorizontalPlane HPlane;

#endif
