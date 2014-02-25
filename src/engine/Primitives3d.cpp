#include "Primitives3d.h"

// TODO: use delegating constructors
Cuboid::Cuboid(const Point3f &_center, float x, float y, float z, SDL_Color _color)
: BoundingBox(_center, x, y, z), color(_color), textureID(0), transformer(_center) {

	originalColor = color;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLfloat vertexData[] = {
		//  X     Y     Z       U     V
		// UV are remapped, so loads image texture as it is
		// UVs are much easier to use in this state rather than indices

		// bottom
		-_halfDistX, -_halfDistY, -_halfDistZ, 0.0f, 0.0f,
		_halfDistX, -_halfDistY, -_halfDistZ, 1.0f, 0.0f,
		-_halfDistX, -_halfDistY, _halfDistZ, 0.0f, 1.0f,
		_halfDistX, -_halfDistY, -_halfDistZ, 1.0f, 0.0f,
		_halfDistX, -_halfDistY, _halfDistZ, 1.0f, 1.0f,
		-_halfDistX, -_halfDistY, _halfDistZ, 0.0f, 1.0f,

		// top
		-_halfDistX, _halfDistY, -_halfDistZ, 0.0f, 1.0f,
		-_halfDistX, _halfDistY, _halfDistZ, 0.0f, 0.0f,
		_halfDistX, _halfDistY, -_halfDistZ, 1.0f, 1.0f,
		_halfDistX, _halfDistY, -_halfDistZ, 1.0f, 1.0f,
		-_halfDistX, _halfDistY, _halfDistZ, 0.0f, 0.0f,
		_halfDistX, _halfDistY, _halfDistZ, 1.0f, 0.0f,

		// back
		-_halfDistX, -_halfDistY, _halfDistZ, 1.0f, 1.0f,
		_halfDistX, -_halfDistY, _halfDistZ, 0.0f, 1.0f,
		-_halfDistX, _halfDistY, _halfDistZ, 1.0f, 0.0f,
		_halfDistX, -_halfDistY, _halfDistZ, 0.0f, 1.0f,
		_halfDistX, _halfDistY, _halfDistZ, 0.0f, 0.0f,
		-_halfDistX, _halfDistY, _halfDistZ, 1.0f, 0.0f,

		// front
		-_halfDistX, -_halfDistY, -_halfDistZ, 0.0f, 1.0f,
		-_halfDistX, _halfDistY, -_halfDistZ, 0.0f, 0.0f,
		_halfDistX, -_halfDistY, -_halfDistZ, 1.0f, 1.0f,
		_halfDistX, -_halfDistY, -_halfDistZ, 1.0f, 1.0f,
		-_halfDistX, _halfDistY, -_halfDistZ, 0.0f, 0.0f,
		_halfDistX, _halfDistY, -_halfDistZ, 1.0f, 0.0f,

		// left
		-_halfDistX, -_halfDistY, _halfDistZ, 0.0f, 1.0f,
		-_halfDistX, _halfDistY, -_halfDistZ, 1.0f, 0.0f,
		-_halfDistX, -_halfDistY, -_halfDistZ, 1.0f, 1.0f,
		-_halfDistX, -_halfDistY, _halfDistZ, 0.0f, 1.0f,
		-_halfDistX, _halfDistY, _halfDistZ, 0.0f, 0.0f,
		-_halfDistX, _halfDistY, -_halfDistZ, 1.0f, 0.0f,

		// right
		_halfDistX, -_halfDistY, _halfDistZ, 1.0f, 1.0f,
		_halfDistX, -_halfDistY, -_halfDistZ, 0.0f, 1.0f,
		_halfDistX, _halfDistY, -_halfDistZ, 0.0f, 0.0f,
		_halfDistX, -_halfDistY, _halfDistZ, 1.0f, 1.0f,
		_halfDistX, _halfDistY, -_halfDistZ, 0.0f, 0.0f,
		_halfDistX, _halfDistY, _halfDistZ, 1.0f, 0.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	compileShaders();
}

Cuboid::Cuboid(const Point3f &_center, float x, float y, float z, GLuint _textureID)
: BoundingBox(_center, x, y, z), textureID(_textureID), transformer(_center) {
	color = SDL_COLOR_GRAY;
	originalColor = color;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLfloat vertexData[] = {
		//  X     Y     Z       U     V
		// UV are remapped, so loads image texture as it is
		// UVs are much easier to use in this state rather than indices

		// bottom
		-_halfDistX, -_halfDistY, -_halfDistZ, 0.0f, 0.0f,
		_halfDistX, -_halfDistY, -_halfDistZ, 1.0f, 0.0f,
		-_halfDistX, -_halfDistY, _halfDistZ, 0.0f, 1.0f,
		_halfDistX, -_halfDistY, -_halfDistZ, 1.0f, 0.0f,
		_halfDistX, -_halfDistY, _halfDistZ, 1.0f, 1.0f,
		-_halfDistX, -_halfDistY, _halfDistZ, 0.0f, 1.0f,

		// top
		-_halfDistX, _halfDistY, -_halfDistZ, 0.0f, 1.0f,
		-_halfDistX, _halfDistY, _halfDistZ, 0.0f, 0.0f,
		_halfDistX, _halfDistY, -_halfDistZ, 1.0f, 1.0f,
		_halfDistX, _halfDistY, -_halfDistZ, 1.0f, 1.0f,
		-_halfDistX, _halfDistY, _halfDistZ, 0.0f, 0.0f,
		_halfDistX, _halfDistY, _halfDistZ, 1.0f, 0.0f,

		// back
		-_halfDistX, -_halfDistY, _halfDistZ, 1.0f, 1.0f,
		_halfDistX, -_halfDistY, _halfDistZ, 0.0f, 1.0f,
		-_halfDistX, _halfDistY, _halfDistZ, 1.0f, 0.0f,
		_halfDistX, -_halfDistY, _halfDistZ, 0.0f, 1.0f,
		_halfDistX, _halfDistY, _halfDistZ, 0.0f, 0.0f,
		-_halfDistX, _halfDistY, _halfDistZ, 1.0f, 0.0f,

		// front
		-_halfDistX, -_halfDistY, -_halfDistZ, 0.0f, 1.0f,
		-_halfDistX, _halfDistY, -_halfDistZ, 0.0f, 0.0f,
		_halfDistX, -_halfDistY, -_halfDistZ, 1.0f, 1.0f,
		_halfDistX, -_halfDistY, -_halfDistZ, 1.0f, 1.0f,
		-_halfDistX, _halfDistY, -_halfDistZ, 0.0f, 0.0f,
		_halfDistX, _halfDistY, -_halfDistZ, 1.0f, 0.0f,

		// left
		-_halfDistX, -_halfDistY, _halfDistZ, 0.0f, 1.0f,
		-_halfDistX, _halfDistY, -_halfDistZ, 1.0f, 0.0f,
		-_halfDistX, -_halfDistY, -_halfDistZ, 1.0f, 1.0f,
		-_halfDistX, -_halfDistY, _halfDistZ, 0.0f, 1.0f,
		-_halfDistX, _halfDistY, _halfDistZ, 0.0f, 0.0f,
		-_halfDistX, _halfDistY, -_halfDistZ, 1.0f, 0.0f,

		// right
		_halfDistX, -_halfDistY, _halfDistZ, 1.0f, 1.0f,
		_halfDistX, -_halfDistY, -_halfDistZ, 0.0f, 1.0f,
		_halfDistX, _halfDistY, -_halfDistZ, 0.0f, 0.0f,
		_halfDistX, -_halfDistY, _halfDistZ, 1.0f, 1.0f,
		_halfDistX, _halfDistY, -_halfDistZ, 0.0f, 0.0f,
		_halfDistX, _halfDistY, _halfDistZ, 1.0f, 0.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	compileShaders();
}

GLuint Cuboid::createShader(const char * shaderCode, GLenum shaderType) {
	GLuint shader = glCreateShader(shaderType);
	if (shader == 0) {
		// couldn't create shader, how do we wanna exit?
	}

	const GLchar* strings[1];	// shader code strings
	strings[0] = shaderCode;

	GLint lengths[1];			// shader code lengths
	lengths[0] = strlen(shaderCode);

	glShaderSource(shader, 1, strings, lengths);
	glCompileShader(shader);

	GLint ok;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
	if (!ok) {
		std::cout << "Failed compile" << std::endl;
		/*
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);	// use here to determine error
		*/
		getchar();
	}

	return shader;
}

// not used
GLuint Cuboid::createBuffer(GLenum bufferType, const void *bufferData, GLsizei bufferSize) {
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(bufferType, buffer);
	glBufferData(bufferType, bufferSize, bufferData, GL_STATIC_DRAW);
	return buffer;
}

void Cuboid::compileShaders() {
	GLuint program = glCreateProgram();
	if (0 == program) {
		// bad
	}

	glAttachShader(program, createShader(vertexShaderCode, GL_VERTEX_SHADER));
	glAttachShader(program, createShader(fragmentShaderCode, GL_FRAGMENT_SHADER));
	glLinkProgram(program);

	GLint ok;
	glGetProgramiv(program, GL_LINK_STATUS, &ok);
	//GLchar ErrorLog[1024] = { 0 };
	if (!ok) {
		//glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		//fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
	}

	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &ok);
	if (!ok) {
		//glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		//fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
	}

	glUseProgram(program);

	mvpLocation = glGetUniformLocation(program, "mvp");
	colorLocation = glGetUniformLocation(program, "color");
	textureIDLocation = glGetUniformLocation(program, "sampler");
	useTextureLocation = glGetUniformLocation(program, "useTexture");
}

void Cuboid::draw() {
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

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);	// x,y,z, repeat every 5 values

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));	// u,v start at 3, repeat 5

	glDrawArrays(GL_TRIANGLES, 0, 36);	/// cuboid 12*3

	// unbind everything
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Cuboid::setColor(SDL_Color _color) {
	color = _color;
}

void Cuboid::resetColor() {
	color = originalColor;
}

void Cuboid::setTexture(GLuint texture) {
	textureID = texture;
}

// TODO: since object size is now different we need to adapt bbox to it
void Cuboid::scale(float x, float y, float z) {
	transformer.scale += Vector3f(x, y, z);
}

void Cuboid::setScale(float x, float y, float z) {
	transformer.scale = Vector3f(x, y, z);
}

Vector3f Cuboid::getScale() {
	return transformer.scale;
}

void Cuboid::rotate(float x, float y, float z) {
	transformer.rotate += Vector3f(x, y, z);
	// not changing bbox here
}

void Cuboid::setRotate(float x, float y, float z) {
	transformer.rotate = Vector3f(x, y, z);
}

void Cuboid::setCenter(float x, float y, float z) {
	center = Point3f(x, y, z);
	transformer.center = Vector3f(x, y, z);
	// only changed center point no need to change bbox
}

// or the other way around
void Cuboid::setCenter(const Point3f & _center) {
	setCenter(_center.x, _center.y, _center.z);
}

void Cuboid::move(const Vector3f & v) {
	center += v;
	transformer.center = Vector3f(center.x, center.y, center.z);
}

Point3f Cuboid::getCenter() {
	return BoundingBox::getCenter();
}

/*bool Plane::collidesWith(const BoundingBox & box) {
	Rect plane = { (int)(center.x - halfDistX.x), (int)(center.z - halfDistZ.z), (int)(2 * halfDistX.x), (int)(2 * halfDistZ.z) };
	Point2 p = { (int)box.getCenter().x, (int)box.getCenter().z };*/

	// further check the whole plane around center point

	/*return center.y > box.getCenter().y - box.getHalfDistY() - 0.05
	&& center.y < box.getCenter().y - box.getHalfDistY() + 0.05
	&& plane.contains(p);*/
	// TODO: do something else ?
	/*return box.getCenter().y - box.getHalfDistY() >= 0 && box.getCenter().y - box.getHalfDistY() <= 0.01 && plane.contains(p);
}*/