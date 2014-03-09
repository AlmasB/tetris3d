#include "MD3Object.h"

MD3Object::MD3Object(std::string fileName)
: Primitive3d(Point3f(-70, 0, 0), 5, 5, 5, 0) {

	MD3Loader loader;
	loader.loadModel(fileName, vertices, indices, numVertices, numIndices);

	vbo = createBuffer(GL_ARRAY_BUFFER, vertices, numVertices*sizeof(GLfloat));
	ibo = createBuffer(GL_ELEMENT_ARRAY_BUFFER, indices, numIndices*sizeof(GLushort));	
}

MD3Object::~MD3Object() {
	delete vertices;
	delete indices;
}

void MD3Object::draw() {
	glUseProgram(shaderProgram);

	glUniformMatrix4fv(mvpLocation, 1, GL_TRUE, (const GLfloat*)transformer.transform());
	glUniform4f(colorLocation, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f);	// TODO: pre-calculate float values

	if (0 != textureID) {	// check if texture exists
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