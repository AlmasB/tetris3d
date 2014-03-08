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
		for (int i = 0; i < header.numSurfaces; ++i) {
			file.seekg(offsetSurfaces, std::ios::beg);
			file.read((char *)&surfaces[i], sizeof(Surface));


			offsetSurfaces += surfaces[i].offsetEnd;
			std::cout << "offset surf " << offsetSurfaces << std::endl;
		}


		

		for (int i = 0; i < header.numSurfaces; ++i) {
			std::cout << surfaces[i].ident << std::endl;
			std::cout << surfaces[i].name << std::endl;
			std::cout << surfaces[i].numTriangles << std::endl;
			std::cout << surfaces[i].numVertices << std::endl;
			std::cout << "end offset: " << surfaces[i].offsetEnd << std::endl;
		}


		file.close();
		delete surfaces;
	}
	else {
		std::cout << "Failed to open file: " << fileName << std::endl;
	}

}