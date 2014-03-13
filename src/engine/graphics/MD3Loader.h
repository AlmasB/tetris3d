#ifndef __MD3_LOADER_H__
#define __MD3_LOADER_H__

#include <string>
#include <fstream>

#include <GL/glew.h>

#include "../math/GameMath.h"
#include "../core/Debug.h"
#include "MD3Object.h"



class MD3Loader {
	private:
	public:
		void loadModel(std::string fileName, GLfloat * &vertices, GLushort * &indices, int &numVertices, int &numIndices);
		//MD3Object loadModel(const std::string & fileName);
};

#endif
