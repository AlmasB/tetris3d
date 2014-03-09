#ifndef __MD3OBJECT_H__
#define __MD3OBJECT_H__

#include "Primitives3d.h"
#include "MD3Loader.h"

class MD3Object : public Primitive3d {
	private:
		GLfloat * vertices;
		GLushort * indices;

		int numVertices;
		int numIndices;
	public:
		MD3Object(std::string fileName);
		~MD3Object();
		void draw();
};

#endif