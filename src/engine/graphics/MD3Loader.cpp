#include "MD3Loader.h"

void MD3Loader::loadModel(std::string fileName) {
	debug("Loading model: ");
	debug(fileName.c_str());

	std::ifstream file;
	file.open(fileName, std::ios::in | std::ios::binary);

	if (file) {
		file.seekg(0, file.end);
		int size = (int) file.tellg();
		file.seekg(0, file.beg);

		std::cout << "file size: " << size << std::endl;

		MD3Header header;
		file.read((char *) &header, sizeof(MD3Header));
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
		for (int i = 0; i < 1; ++i) {	// read first surface (mesh)
			file.seekg(offsetSurfaces, std::ios::beg);
			file.read((char *)&surfaces[i], sizeof(Surface));



			Triangle * tris = new Triangle[surfaces[0].numTriangles];
			file.seekg(offsetSurfaces + surfaces[0].offsetTriangles, std::ios::beg);
			file.read((char*)tris, sizeof(Triangle) * surfaces[0].numTriangles);
			
			std::cout << " TRIANGLES: " << std::endl;

			for (int j = 0; j < surfaces[0].numTriangles; ++j) {
				std::cout << tris[j].indices[0] << " " << tris[j].indices[1] << " " << tris[j].indices[2] << std::endl;
			}


			Vertex * vertices = new Vertex[surfaces[0].numVertices];
			file.seekg(offsetSurfaces + surfaces[0].offsetXYZ, std::ios::beg);
			file.read((char*)vertices, sizeof(Vertex)* surfaces[0].numVertices);

			std::cout << " VERTICES: " << std::endl;

			for (int j = 0; j < surfaces[0].numVertices; ++j) {
				std::cout << vertices[j].x << " " << vertices[j].y << " " << vertices[j].z << std::endl;
			}


			offsetSurfaces += surfaces[i].offsetEnd;
			std::cout << "offset surf " << offsetSurfaces << std::endl;

			delete tris;
		}


		
		

		/*for (int i = 0; i < header.numSurfaces; ++i) {
			std::cout << surfaces[i].ident << std::endl;
			std::cout << surfaces[i].name << std::endl;
			std::cout << surfaces[i].numTriangles << std::endl;
			std::cout << surfaces[i].numVertices << std::endl;
			std::cout << "end offset: " << surfaces[i].offsetEnd << std::endl;

		}*/


		std::cout << " END OF SURFACE DATA KIND OF" << std::endl;

	
		

		file.close();
		//delete tris;
		delete surfaces;
	}
	else {
		std::cout << "Failed to open file: " << fileName << std::endl;
	}

}

void MD3Loader::loadModel(std::string fileName, GLfloat * &vertices, GLushort * &indices, int &numVertices, int &numIndices) {
	debug("Loading model: ");
	debug(fileName.c_str());

	std::ifstream file;
	file.open(fileName, std::ios::in | std::ios::binary);

	if (file) {
		file.seekg(0, file.end);
		int size = (int)file.tellg();
		file.seekg(0, file.beg);

		std::cout << "file size: " << size << std::endl;

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
		for (int i = 0; i < 1; ++i) {	// read first surface (mesh)
			file.seekg(offsetSurfaces, std::ios::beg);
			file.read((char *)&surfaces[i], sizeof(Surface));



			Triangle * tris = new Triangle[surfaces[0].numTriangles];
			file.seekg(offsetSurfaces + surfaces[0].offsetTriangles, std::ios::beg);
			file.read((char*)tris, sizeof(Triangle)* surfaces[0].numTriangles);

			std::cout << " TRIANGLES: " << std::endl;

			numIndices = surfaces[0].numTriangles * 3;
			indices = new GLushort[surfaces[0].numTriangles * 3];
			for (int j = 0; j < surfaces[0].numTriangles; ++j) {
				indices[j * 3] = tris[j].indices[0];
				indices[j * 3 + 1] = tris[j].indices[1];
				indices[j * 3 + 2] = tris[j].indices[2];
			}


			Vertex * verticesLocal = new Vertex[surfaces[0].numVertices];
			file.seekg(offsetSurfaces + surfaces[0].offsetXYZ, std::ios::beg);
			file.read((char*)verticesLocal, sizeof(Vertex)* surfaces[0].numVertices);

			std::cout << " VERTICES: " << std::endl;

			numVertices = surfaces[0].numVertices * 3;
			vertices = new GLfloat[surfaces[0].numVertices * 3];

			for (int j = 0; j < surfaces[0].numVertices; ++j) {
				vertices[3 * j] = verticesLocal[j].x / 100.0f;
				vertices[3 * j + 1] = verticesLocal[j].y / 100.0f;
				vertices[3 * j + 2] = verticesLocal[j].z /  100.0f;
			}


			offsetSurfaces += surfaces[i].offsetEnd;
			std::cout << "offset surf " << offsetSurfaces << std::endl;


		}





		/*for (int i = 0; i < header.numSurfaces; ++i) {
		std::cout << surfaces[i].ident << std::endl;
		std::cout << surfaces[i].name << std::endl;
		std::cout << surfaces[i].numTriangles << std::endl;
		std::cout << surfaces[i].numVertices << std::endl;
		std::cout << "end offset: " << surfaces[i].offsetEnd << std::endl;

		}*/


		std::cout << " END OF SURFACE DATA KIND OF" << std::endl;




		file.close();
		//delete tris;
		//delete surfaces;
	}
	else {
		std::cout << "Failed to open file: " << fileName << std::endl;
	}
}