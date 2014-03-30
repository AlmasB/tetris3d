#include "MD3Object.h"

MD3Object::MD3Object()
: Primitive3d(Point3f(0, 0, 0), 5, 5, 5, 0) {}

MD3Object::~MD3Object() {
	delete vertices;
	delete indices;
}

void MD3Object::draw() {
	glUseProgram(shaderProgram);

	glUniformMatrix4fv(mvpLocation, 1, GL_TRUE, (const GLfloat*)transformer.transform());
	glUniform4f(colorLocation, color.r, color.g, color.b, 1.0f);

	if (textureID != 0) {	// check if texture exists
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glUniform1i(useTextureLocation, 1);		// use texture (1) or use color (0)
	}
	else {
		glUniform1i(useTextureLocation, 0);
	}

	glUniform1i(useUI, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_SHORT, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);

	glUseProgram(0);
}

void MD3Object::loadImpl(const std::string & fileName) {
#ifdef __DEBUG
	debug("Loading MD3 model:", fileName.c_str());
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
		if (header.ident != 860898377 /*IDP3*/ || header.version != 15) {
#ifdef __DEBUG
			debug("MD3 file ident", header.ident);
			debug("MD3 file version", header.version);
#endif
			file.close();
			throw EngineException("MD3 file wrong ID or version", fileName.c_str());
		}

		/*std::cout << header.name << std::endl;
		std::cout << header.numFrames << std::endl;
		std::cout << header.numTags << std::endl;
		std::cout << header.numSurfaces << std::endl;
		std::cout << header.offsetEOF << std::endl;
		std::cout << header.flags << std::endl;*/


		int offsetSurfaces = header.offsetSurfaces;
		//std::cout << "offset surf " << offsetSurfaces << std::endl;

		Surface * surfaces = new Surface[header.numSurfaces];
		for (int i = 0; i < header.numSurfaces; ++i) {
			file.seekg(offsetSurfaces, std::ios::beg);
			file.read((char *)&surfaces[i], sizeof(Surface));

			Triangle * tris = new Triangle[surfaces[i].numTriangles];
			file.seekg(offsetSurfaces + surfaces[i].offsetTriangles, std::ios::beg);
			file.read((char*)tris, sizeof(Triangle)* surfaces[i].numTriangles);

			//std::cout << " TRIANGLES: " << std::endl;

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

			//std::cout << " VERTICES: " << std::endl;

			numVertices = surfaces[i].numVertices * 3;
			vertices = new GLfloat[surfaces[i].numVertices * 3];
			for (int j = 0; j < surfaces[i].numVertices; ++j) {
				vertices[3 * j] = verticesLocal[j].x / 100.0f;	// scale it down
				vertices[3 * j + 1] = verticesLocal[j].y / 100.0f;
				vertices[3 * j + 2] = verticesLocal[j].z / 100.0f;
			}


			offsetSurfaces += surfaces[i].offsetEnd;
			//std::cout << "offset surf " << offsetSurfaces << std::endl;

			delete[] verticesLocal;
			delete[] tris;
		}

#ifdef __DEBUG
		debug("Finished reading:", fileName.c_str());
#endif

		file.close();
		delete[] surfaces;
	}
	else {
		throw EngineException("MD3 Failed to open", fileName.c_str());
	}

	// init VBO, IBO

	vbo = createBuffer(GL_ARRAY_BUFFER, vertices, numVertices*sizeof(GLfloat));
	ibo = createBuffer(GL_ELEMENT_ARRAY_BUFFER, indices, numIndices*sizeof(GLushort));
}

std::shared_ptr<MD3Object> MD3Object::load(const std::string & fileName) {
	std::shared_ptr<MD3Object> obj = std::shared_ptr<MD3Object>(new MD3Object());
	try {
		obj->loadImpl(fileName);
	}
	catch (EngineException& e) {
		throw;
	}

	return obj;
}