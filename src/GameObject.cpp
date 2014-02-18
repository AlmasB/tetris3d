#include "GameObject.h"

GameObject::GameObject(const Point3 & _center, float x, float y, float z, RGBColor _color)
	: BoundingBox(_center, x, y, z) {
	alive = true;
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

void GameObject::compileShaders() {
	GLuint program = glCreateProgram();
	if (program == 0) {
		// bad
	}

	glAttachShader(program, createShader(pVS, GL_VERTEX_SHADER));
	glAttachShader(program, createShader(pFS, GL_FRAGMENT_SHADER));
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

void GameObject::draw(std::shared_ptr<Camera> cam) {
	Pipeline p;
	//p.Rotate(0.0f, Scale, 0.0f);
	//p.WorldPos(0.0f, 0.0f, 7.0f);
	p.WorldPos(center.getX(), center.getY(), center.getZ());

	p.SetCamera(cam->GetPos(), cam->GetTarget(), cam->GetUp());
	p.SetPerspectiveProj(60.0f, 800, 600, 1.0f, 100.0f);	// 45.0 is a good value

	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans());

	if (!locked)
		glUniform4f(mycolor, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f);	// color changer
	else
		glUniform4f(mycolor, COLOR_YELLOW.r / 255.0f, COLOR_YELLOW.g / 255.0f, COLOR_YELLOW.b / 255.0f, 1.0f);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//glDrawElements(GL_TRIANGLES, 3 * 12, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 3 * numOfTriangles, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
}

void GameObject::move(const Vector3 & v) {
	center += v;
}

void GameObject::setDistZ(float dist) {
	halfDistZ = Vector3(0, 0, dist / 2.0f);
}

void GameObject::setLocked(bool b) {
	locked = b;
}

Cube::Cube(const Point3 & _center, RGBColor _color) : GameObject(_center, 2, 2, 2, _color) {
	numOfTriangles = 12;
	//vertex buffer

	Vector3f Vertices[8];
    Vertices[0] = Vector3f(-1.0f, -1.0f, 1.0f);
    Vertices[1] = Vector3f(1.0f, -1.0f, 1.0f);
    Vertices[2] = Vector3f(1.0f, 1.0f, 1.0);
    Vertices[3] = Vector3f(-1.0f, 1.0f, 1.0f);

	Vertices[4] = Vector3f(-1.0f, -1.0f, -1.0f);
	Vertices[5] = Vector3f(1.0f, -1.0f, -1.0f);
	Vertices[6] = Vector3f(1.0f, 1.0f, -1.0f);
	Vertices[7] = Vector3f(-1.0f, 1.0f, -1.0f);


	VBO = createBuffer(GL_ARRAY_BUFFER, Vertices, sizeof(Vertices));

	//element buffer
	unsigned int Indices[] = {
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

	EBO = createBuffer(GL_ELEMENT_ARRAY_BUFFER, Indices, sizeof(Indices));

	compileShaders();
}

GLuint GameObject::createBuffer(GLenum bufferType, const void *bufferData, GLsizei bufferSize) {
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(bufferType, buffer);
	glBufferData(bufferType, bufferSize, bufferData, GL_STATIC_DRAW);
	return buffer;
}

HorizontalPlane::HorizontalPlane(const Point3 & c, float x, float y, float z, RGBColor _color)
: GameObject(c, x, y, z, _color) {

	numOfTriangles = 12;

	/*Vector3f Vertices[3];
	Vertices[0] = Vector3f(x/2, 0.0f, z/2);
	Vertices[1] = Vector3f(-x/2, 0.0f, z/2);
	Vertices[2] = Vector3f(-x/2, 0.0f, -z/2);
	//Vertices[3] = Vector3f(x/2, 0.0f, -z/2);

	cout << "assigned vectors" << endl;

	glGenBuffers(1, &VBO);

	cout << "genBuffers finished" << endl;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	cout << "bind buffer finished " << endl;
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);



	cout << "VErtex PLANE created" << endl;

	//element buffer
	unsigned int Indices[] = { 5, 1, 2
		//2, 0, 3 
	};

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	CompileShaders();*/

	Vector3f Vertices[8];
	Vertices[0] = Vector3f(-x/2, -y/2, z/2);
	Vertices[1] = Vector3f(x/2, -y/2, z/2);
	Vertices[2] = Vector3f(x/2, y/2, z/2);
	Vertices[3] = Vector3f(-x/2, y/2, z/2);

	Vertices[4] = Vector3f(-x/2, -y/2, -z/2);
	Vertices[5] = Vector3f(x/2, -y/2, -z/2);
	Vertices[6] = Vector3f(x/2, y/2, -z/2);
	Vertices[7] = Vector3f(-x/2, y/2, -z/2);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	//element buffer
	unsigned int Indices[] = { 0, 1, 2,
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
		6, 2, 1 };


	EBO = createBuffer(GL_ELEMENT_ARRAY_BUFFER, Indices, sizeof(Indices));

	/*glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);*/

	compileShaders();
}


bool HorizontalPlane::collidesWith(const BoundingBox & box) {
	Rect plane = { (int)(center.getX()-halfDistX.getX()), (int)(center.getZ()-halfDistZ.getZ()), (int)(2*halfDistX.getX()), (int)(2*halfDistZ.getZ()) };
	Point2 p = { (int)box.center.getX(), (int)box.center.getZ() };

	// further check the whole plane around center point

	return center.getY() > box.center.getY() - box.halfDistY.getY() - 0.05
		&& center.getY() < box.center.getY() - box.halfDistY.getY() + 0.05
		&& plane.contains(p);
}
