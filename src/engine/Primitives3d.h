#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__

#include "PhysicsEngine.h"
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

// TODO: once there's an actual 3d model, md2, md3 or whatever
// most of the stuff will go to a Primitive class, of which cuboid will be just a subclass
// but for now this is my primitive
// BIG TODO: maybe we could subtract half distances of object, so that when we use create obj
// at 0.0.0 it starts drawing from 0.0.0 and not half of it, x-x0 / 2 gives us center etc

class Cuboid : public PhysicsObject, public Movable {
	protected:
		GLuint shaderProgram;

		GLuint vbo;	// vertex buffer object
		CameraTransformer transformer;

		SDL_Color originalColor;
		SDL_Color color;
		GLuint colorLocation;

		GLuint textureID;
		GLuint textureIDLocation;
		GLuint useTextureLocation;	// we pass 1 to use texture and 0 to use color

		GLuint mvpLocation;	// model view projection
		GLuint useUI;	// TODO: rename

		GLuint createBuffer(GLenum, const void *, GLsizei);
	public:
		Cuboid(const Point3f &center, float lengthX, float lengthY, float lengthZ, SDL_Color color);
		Cuboid(const Point3f &center, float lengthX, float lengthY, float lengthZ, SDL_Color color, GLuint textureID = 0);

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

		void draw();
};

#endif