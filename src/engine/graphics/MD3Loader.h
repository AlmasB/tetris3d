#ifndef __MD3_LOADER_H__
#define __MD3_LOADER_H__

#include <string>
#include <fstream>

#include "../math/GameMath.h"
#include "../core/Debug.h"

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
	Vector3f localOrigin;
	float radius;
	unsigned char name[16];
};

struct Tag {
	unsigned char name[64];
	Vector3f origin;
	Vector3f axis[3];
};

struct Surface {
	int ident;
	unsigned char name[64];
	int numFrames;
	int numShaders;
	int numVertices;
	int numTriangles;
	int offsetTriangles;
	int offsetShaders;
	int offsetST;
	int offsetXYZ;
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
		void loadModel(std::string fileName);
};

#endif