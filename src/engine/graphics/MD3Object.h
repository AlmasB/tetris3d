#ifndef __MD3OBJECT_H__
#define __MD3OBJECT_H__

#include "Primitives3d.h"
#include "MD3Loader.h"

class MD3Object : public Cuboid {
	private:
		GLuint vbo2, ibo2;
		GLfloat * vertices;
		GLushort * indices;

		int numVertices;
		int numIndices;
	public:
		MD3Object(std::string fileName);
		void draw();
};

#endif