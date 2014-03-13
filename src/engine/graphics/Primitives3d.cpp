#include "Primitives3d.h"

Primitive3d::Primitive3d(const Point3f & center, float x, float y, float z, SDL_Color color)
: Primitive3d(center, x, y, z, color, 0) {}

Primitive3d::Primitive3d(const Point3f & center, float x, float y, float z, GLuint textureID)
: Primitive3d(center, x, y, z, SDL_COLOR_GRAY, textureID) {}

Primitive3d::Primitive3d(const Point3f &_center, float _x, float _y, float _z, SDL_Color color, GLuint _textureID)
: PhysicsObject(_center, _x, _y, _z), textureID(_textureID), transformer(_center), color(color), originalColor(color) {

	shaderProgram = ShaderManager::getInstance()->createProgram(vertexShaderCode, fragmentShaderCode);

	glUseProgram(shaderProgram);	// TODO: check if needed here

	mvpLocation = glGetUniformLocation(shaderProgram, "mvp");
	colorLocation = glGetUniformLocation(shaderProgram, "color");
	textureIDLocation = glGetUniformLocation(shaderProgram, "sampler");
	useTextureLocation = glGetUniformLocation(shaderProgram, "useTexture");

	useUI = glGetUniformLocation(shaderProgram, "useUI");

	glUseProgram(0);
}

GLuint Primitive3d::createBuffer(GLenum bufferType, const void *bufferData, GLsizei bufferSize) {
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(bufferType, buffer);
	glBufferData(bufferType, bufferSize, bufferData, GL_STATIC_DRAW);
	glBindBuffer(bufferType, 0);	// unbind
	return buffer;
}

void Primitive3d::setColor(SDL_Color _color) {
	color = _color;
}

void Primitive3d::resetColor() {
	color = originalColor;
}

void Primitive3d::setTexture(GLuint texture) {
	textureID = texture;
}

// TODO: since object size is now different we need to adapt phys obj to it
void Primitive3d::scale(float x, float y, float z) {
	transformer.scale += Vector3f(x, y, z);
}

void Primitive3d::setScale(float x, float y, float z) {
	transformer.scale = Vector3f(x, y, z);
}

Vector3f Primitive3d::getScale() {
	return transformer.scale;
}

void Primitive3d::rotate(float x, float y, float z) {
	transformer.rotate += Vector3f(x, y, z);
	// not changing bbox here
}

void Primitive3d::setRotate(float x, float y, float z) {
	transformer.rotate = Vector3f(x, y, z);
}

void Primitive3d::setCenter(float x, float y, float z) {
	center = Point3f(x, y, z);
	transformer.center = Vector3f(x, y, z);
	// only changed center point no need to change bbox
}

// or the other way around
void Primitive3d::setCenter(const Point3f & _center) {
	setCenter(_center.x, _center.y, _center.z);
}

void Primitive3d::move(const Vector3f & v) {
	center += v;
	transformer.center = Vector3f(center.x, center.y, center.z);
}

Point3f Primitive3d::getCenter() {
	return PhysicsObject::getCenter();
}

/* CUBOID CLASS DEFINITION */

Cuboid::Cuboid(const Point3f & center, float x, float y, float z, SDL_Color color)
: Cuboid(center, x, y, z, color, 0) {}

Cuboid::Cuboid(const Point3f & center, float x, float y, float z, GLuint textureID)
: Cuboid(center, x, y, z, SDL_COLOR_GRAY, textureID) {}

Cuboid::Cuboid(const Point3f & center, float lX, float lY, float lZ, SDL_Color color, GLuint textureID)
: Primitive3d(center, lX, lY, lZ, color, textureID) {

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	GLfloat vertexData[] = {
		//  X     Y     Z       U     V
		// UV are remapped, so loads image texture as it is
		// UVs are much easier to use in this state rather than indices

		// bottom
		-hlX, -hlY, -hlZ, 0.0f, 0.0f,
		hlX, -hlY, -hlZ, 1.0f, 0.0f,
		-hlX, -hlY, hlZ, 0.0f, 1.0f,
		hlX, -hlY, -hlZ, 1.0f, 0.0f,
		hlX, -hlY, hlZ, 1.0f, 1.0f,
		-hlX, -hlY, hlZ, 0.0f, 1.0f,

		// top
		-hlX, hlY, -hlZ, 0.0f, 1.0f,
		-hlX, hlY, hlZ, 0.0f, 0.0f,
		hlX, hlY, -hlZ, 1.0f, 1.0f,
		hlX, hlY, -hlZ, 1.0f, 1.0f,
		-hlX, hlY, hlZ, 0.0f, 0.0f,
		hlX, hlY, hlZ, 1.0f, 0.0f,

		// back
		-hlX, -hlY, hlZ, 1.0f, 1.0f,
		hlX, -hlY, hlZ, 0.0f, 1.0f,
		-hlX, hlY, hlZ, 1.0f, 0.0f,
		hlX, -hlY, hlZ, 0.0f, 1.0f,
		hlX, hlY, hlZ, 0.0f, 0.0f,
		-hlX, hlY, hlZ, 1.0f, 0.0f,

		// front
		-hlX, -hlY, -hlZ, 0.0f, 1.0f,
		-hlX, hlY, -hlZ, 0.0f, 0.0f,
		hlX, -hlY, -hlZ, 1.0f, 1.0f,
		hlX, -hlY, -hlZ, 1.0f, 1.0f,
		-hlX, hlY, -hlZ, 0.0f, 0.0f,
		hlX, hlY, -hlZ, 1.0f, 0.0f,

		// left
		-hlX, -hlY, hlZ, 0.0f, 1.0f,
		-hlX, hlY, -hlZ, 1.0f, 0.0f,
		-hlX, -hlY, -hlZ, 1.0f, 1.0f,
		-hlX, -hlY, hlZ, 0.0f, 1.0f,
		-hlX, hlY, hlZ, 0.0f, 0.0f,
		-hlX, hlY, -hlZ, 1.0f, 0.0f,

		// right
		hlX, -hlY, hlZ, 1.0f, 1.0f,
		hlX, -hlY, -hlZ, 0.0f, 1.0f,
		hlX, hlY, -hlZ, 0.0f, 0.0f,
		hlX, -hlY, hlZ, 1.0f, 1.0f,
		hlX, hlY, -hlZ, 0.0f, 0.0f,
		hlX, hlY, hlZ, 1.0f, 0.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cuboid::draw() {
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);	// x,y,z, repeat every 5 values

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));	// u,v start at 3, repeat 5

	glDrawArrays(GL_TRIANGLES, 0, 36);	// Cuboid 12*3

	// unbind everything
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glUseProgram(0);
}

/* CUBE CLASS DEFINITION */

Cube::Cube(const Point3f & center, float size, SDL_Color color)
: Cuboid(center, size, size, size, color) {}

Cube::Cube(const Point3f & center, float size, GLuint textureID)
: Cuboid(center, size, size, size, textureID) {}