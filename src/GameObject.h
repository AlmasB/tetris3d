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


static const char* vertexShaderCode = "                                                          \n\
#version 120                                                                        \n\
								                                                    \n\
attribute vec3 Position;                                                            \n\
attribute vec2 UV;                                                                                   \n\
uniform mat4 gWVP;                                                                  \n\
varying vec2 outUV;                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWVP * vec4(Position, 1.0);                                       \n\
	outUV = UV;  \n\
}";



static const char* fragmentShaderCode = "                                                          \n\
#version 120                                                                        \n\
varying vec2 outUV;						                                                            \n\
uniform vec4 color;                                                                 \n\
uniform sampler2D sampler;                 \n\
                                                                                  \n\
void main()                                                                         \n\
{                                                                                   \n\
	gl_FragColor = texture2D(sampler, outUV);  					                                        \n\
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

struct Vertex
{
	Vector3f m_pos;
	Vector2f m_tex;

	Vertex() {}

	Vertex(Vector3f pos, Vector2f tex)
	{
		m_pos = pos;
		m_tex = tex;
	}
};

static const GLfloat g_uv_buffer_data[] = {
	0.000103f, 1.0f - 0.336048f,
	0.667979f, 1.0f - 0.335851f,
	0.667969f, 1.0f - 0.671889f,
	1.000004f, 1.0f - 0.671847f,
	0.000059f, 1.0f - 0.000004f,
	0.335973f, 1.0f - 0.335903f,
	1.000023f, 1.0f - 0.000013f,
	0.336024f, 1.0f - 0.671877f
};

class GameObject : public BoundingBox, public Movable {
	private:
	protected:
		RGBColor color;
		uint numOfTriangles;
		
		bool locked;

		CameraTransformer transformer;

		GLuint VBO;
		GLuint VBOtest;
		GLuint IBO;
		GLuint EBO;
		GLuint UVB;

		GLuint gWVPLocation;
		GLuint mycolor;
		
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
		void setCenter(float, float, float);
		void setCenter(const Point3f & center);
		void rotate(float, float, float);
		Point3f getCenter();

		void draw();


		GLuint texture;
		GLuint textureID;

		void test();
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
