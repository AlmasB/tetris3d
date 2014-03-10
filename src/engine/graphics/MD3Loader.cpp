#include "MD3Loader.h"

// TODO: a lot of cleaning
// also we might simply return MD3Object instead of populating its data huh ?
void MD3Loader::loadModel(std::string fileName, GLfloat * &vertices, GLushort * &indices, int &numVertices, int &numIndices) {
#ifdef __DEBUG
	debug("Loading model:", fileName.c_str());
#endif

	std::ifstream file;
	file.open(fileName, std::ios::in | std::ios::binary);

	if (file) {
		file.seekg(0, file.end);
		int size = (int)file.tellg();
		file.seekg(0, file.beg);

#ifdef __DEBUG
		debug("Size:", size);
#endif

		MD3Header header;
		file.read((char *)&header, sizeof(MD3Header));
		std::cout << "ident: " << header.ident << " version: " << header.version << std::endl;
		// 860898377 and 15 or bad file

		std::cout << header.name << std::endl;
		std::cout << header.numFrames << std::endl;
		std::cout << header.numTags << std::endl;
		std::cout << header.numSurfaces << std::endl;
		std::cout << header.offsetEOF << std::endl;
		std::cout << header.flags << std::endl;


		int offsetSurfaces = header.offsetSurfaces;
		std::cout << "offset surf " << offsetSurfaces << std::endl;

		Surface * surfaces = new Surface[header.numSurfaces];
		for (int i = 0; i < header.numSurfaces; ++i) {	
			file.seekg(offsetSurfaces, std::ios::beg);
			file.read((char *)&surfaces[i], sizeof(Surface));



			Triangle * tris = new Triangle[surfaces[i].numTriangles];
			file.seekg(offsetSurfaces + surfaces[i].offsetTriangles, std::ios::beg);
			file.read((char*)tris, sizeof(Triangle)* surfaces[i].numTriangles);

			std::cout << " TRIANGLES: " << std::endl;

			numIndices = surfaces[i].numTriangles * 3;
			indices = new GLushort[surfaces[i].numTriangles * 3];
			for (int j = 0; j < surfaces[i].numTriangles; ++j) {
				indices[j * 3] = tris[j].indices[0];
				indices[j * 3 + 1] = tris[j].indices[1];
				indices[j * 3 + 2] = tris[j].indices[2];
			}


			Vertex * verticesLocal = new Vertex[surfaces[i].numVertices];
			file.seekg(offsetSurfaces + surfaces[i].offsetXYZ, std::ios::beg);
			file.read((char*)verticesLocal, sizeof(Vertex)* surfaces[i].numVertices);

			std::cout << " VERTICES: " << std::endl;

			numVertices = surfaces[i].numVertices * 3;
			vertices = new GLfloat[surfaces[i].numVertices * 3];
			for (int j = 0; j < surfaces[i].numVertices; ++j) {
				vertices[3 * j] = verticesLocal[j].x / 100.0f;
				vertices[3 * j + 1] = verticesLocal[j].y / 100.0f;
				vertices[3 * j + 2] = verticesLocal[j].z /  100.0f;
			}


			offsetSurfaces += surfaces[i].offsetEnd;
			std::cout << "offset surf " << offsetSurfaces << std::endl;

			delete tris;
		}


		std::cout << " END OF SURFACE DATA KIND OF" << std::endl;




		file.close();
		delete surfaces;
	}
	else {
		std::cout << "Failed to open file: " << fileName << std::endl;
	}
}