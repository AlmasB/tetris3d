#ifndef __MD3_LOADER_H__
#define __MD3_LOADER_H__

#include <string>
#include <fstream>

#include <GL/glew.h>

#include "../math/GameMath.h"
#include "../core/Debug.h"
#include "MD3Object.h"

/*
 MD3 Header info from
 http://icculus.org/~phaethon/q3a/formats/md3format.html
 also
 http://www.xbdev.net/3dformats/md3/index.php
*/

struct MD3Header {
	int ident;
	int version;
	unsigned char name[64];
	int flags;
	int numFrames;
	int numTags;
	int numSurfaces;
	int numSkins;
	int offsetFrames;
	int offsetTags;
	int offsetSurfaces;
	int offsetEOF;
};

struct Frame {
	Vector3f minBounds;
	Vector3f maxBounds;
	Vector3f localOrigin;	// center ?
	float radius;	// scale ?
	unsigned char name[16];
};

struct Tag {
	unsigned char name[64];
	Vector3f origin;	// assume center ?
	Vector3f axis[3];	// assume rotation ?
};

struct Surface {
	int ident;
	unsigned char name[64];
	int flags;
	int numFrames;
	int numShaders;
	int numVertices;
	int numTriangles;
	int offsetTriangles;
	int offsetShaders;
	int offsetST;
	int offsetXYZ;	// Vertex
	int offsetEnd;
};

struct Shader {
	unsigned char name[64];
	int index;
};

struct Triangle {
	int indices[3];
};

struct TexCoord {
	float ST[2];
};

struct Vertex {
	short int x;
	short int y;
	short int z;
	short int normal;
};

class MD3Loader {
	private:
	public:
		void loadModel(std::string fileName, GLfloat * &vertices, GLushort * &indices, int &numVertices, int &numIndices);
		//MD3Object loadModel(const std::string & fileName);
};

#endif
