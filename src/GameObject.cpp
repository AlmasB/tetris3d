#include "GameObject.h"

GameObject::GameObject(const Point3f & _center, float x, float y, float z, RGBColor _color)
	: BoundingBox(_center, x, y, z), transformer(CameraTransformer(_center)) {
	locked = false;

	color = _color;
}

GLuint GameObject::createShader(const char * shaderCode, GLenum shaderType) {
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
		cout << "Failed compile" << endl;
		/*
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);	// use here to determine error
		*/
		getchar();
	}

	return shader;
}

GLuint GameObject::createBuffer(GLenum bufferType, const void *bufferData, GLsizei bufferSize) {
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(bufferType, buffer);
	glBufferData(bufferType, bufferSize, bufferData, GL_STATIC_DRAW);
	return buffer;
}

void GameObject::compileShaders() {
	GLuint program = glCreateProgram();
	if (program == 0) {
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

	gWVPLocation = glGetUniformLocation(program, "gWVP");
	mycolor = glGetUniformLocation(program, "color");
}

void GameObject::draw() {
	// calculate 3D position and place it
	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)transformer.transform());

	// give color
	if (!locked)
		glUniform4f(mycolor, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f);
	else
		glUniform4f(mycolor, COLOR_YELLOW.r / 255.0f, COLOR_YELLOW.g / 255.0f, COLOR_YELLOW.b / 255.0f, 1.0f);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 3 * numOfTriangles, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
}

void GameObject::scale(float x, float y, float z) {
	transformer.scale = Vector3f(x, y, z);
}

void GameObject::rotate(float x, float y, float z) {
	transformer.rotate = Vector3f(x, y, z);
}

void GameObject::setCenter(float x, float y, float z) {
	center = Point3f(x, y, z);
	transformer.center = Vector3f(x, y, z);
}
// or the other way around
void GameObject::setCenter(const Point3f & _center) {
	setCenter(_center.x, _center.y, _center.z);
}

void GameObject::move(const Vector3f & v) {
	center += v;
	transformer.center = Vector3f(center.x, center.y, center.z);
}

void GameObject::setDistZ(float dist) {
	halfDistZ = Vector3f(0, 0, dist / 2.0f);
}

void GameObject::setLocked(bool b) {
	locked = b;
}

/* CUBE CLASS DEFINITION BEGIN */

Cube::Cube(const Point3f & _center, const float & size, RGBColor _color) 
: GameObject(_center, size, size, size, _color) {
	numOfTriangles = 12;

	Vector3f vertices[8];
	vertices[0] = Vector3f(-size / 2, -size / 2, size / 2);
	vertices[1] = Vector3f(size / 2, -size / 2, size / 2);
	vertices[2] = Vector3f(size / 2, size / 2, size / 2);
	vertices[3] = Vector3f(-size / 2, size / 2, size / 2);

	vertices[4] = Vector3f(-size / 2, -size / 2, -size / 2);
	vertices[5] = Vector3f(size / 2, -size / 2, -size / 2);
	vertices[6] = Vector3f(size / 2, size / 2, -size / 2);
	vertices[7] = Vector3f(-size / 2, size / 2, -size / 2);

	unsigned int elements[] = {
		0, 1, 2,
		2, 3, 0,
		3, 2, 6,
		6, 7, 3,
		7, 6, 5,
		5, 4, 7,
		4, 5, 1,
		1, 0, 4,
		4, 0, 3,
		3, 7, 4,
		1, 5, 6,
		6, 2, 1 
	};

	VBO = createBuffer(GL_ARRAY_BUFFER, vertices, sizeof(vertices));
	EBO = createBuffer(GL_ELEMENT_ARRAY_BUFFER, elements, sizeof(elements));
	compileShaders();
}

/* PLANE CLASS DEFINITION BEGIN */

Plane::Plane(const Point3f & c, float x, float y, float z, RGBColor _color)
: GameObject(c, x, y, z, _color) {
	numOfTriangles = 12;

	Vector3f vertices[8];
	vertices[0] = Vector3f(-x / 2, -y / 2, z / 2);
	vertices[1] = Vector3f(x / 2, -y / 2, z / 2);
	vertices[2] = Vector3f(x / 2, y / 2, z / 2);
	vertices[3] = Vector3f(-x / 2, y / 2, z / 2);

	vertices[4] = Vector3f(-x / 2, -y / 2, -z / 2);
	vertices[5] = Vector3f(x / 2, -y / 2, -z / 2);
	vertices[6] = Vector3f(x / 2, y / 2, -z / 2);
	vertices[7] = Vector3f(-x / 2, y / 2, -z / 2);

	unsigned int elements[] = { 
		0, 1, 2,
		2, 3, 0,
		3, 2, 6,
		6, 7, 3,
		7, 6, 5,
		5, 4, 7,
		4, 5, 1,
		1, 0, 4,
		4, 0, 3,
		3, 7, 4,
		1, 5, 6,
		6, 2, 1
	};

	VBO = createBuffer(GL_ARRAY_BUFFER, vertices, sizeof(vertices));
	EBO = createBuffer(GL_ELEMENT_ARRAY_BUFFER, elements, sizeof(elements));
	compileShaders();
}

bool Plane::collidesWith(const BoundingBox & box) {
	Rect plane = { (int)(center.x-halfDistX.x), (int)(center.z-halfDistZ.z), (int)(2*halfDistX.x), (int)(2*halfDistZ.z) };
	Point2 p = { (int)box.getCenter().x, (int)box.getCenter().z };

	// further check the whole plane around center point

	return center.y > box.getCenter().y - box.getHalfDistY() - 0.05
		&& center.y < box.getCenter().y - box.getHalfDistY() + 0.05
		&& plane.contains(p);
}
