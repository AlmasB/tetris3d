#include "GameObject.h"

GameObject::GameObject(const Point3 & _center, float x, float y, float z)
	: BoundingBox(_center, x, y, z) {
	alive = true;
	locked = false;

	color = COLOR_BLUE;
}

void GameObject::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		getchar();
		exit(0);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		getchar();
		exit(1);
	}

	glAttachShader(ShaderProgram, ShaderObj);
}

void GameObject::CompileShaders() {
	GLuint ShaderProgram = glCreateProgram();

	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		getchar();
		exit(1);
	}


	AddShader(ShaderProgram, pVS, GL_VERTEX_SHADER);
	AddShader(ShaderProgram, pFS, GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		getchar();
		exit(1);
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		getchar();
		exit(1);
	}

	glUseProgram(ShaderProgram);

	gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");
	assert(gWVPLocation != 0xFFFFFFFF);

	mycolor = glGetUniformLocation(ShaderProgram, "color");
}

void GameObject::draw(std::shared_ptr<Camera> cam) {
	Pipeline p;
	//p.Rotate(0.0f, Scale, 0.0f);
	//p.WorldPos(0.0f, 0.0f, 7.0f);
	p.WorldPos(center.getX(), center.getY(), center.getZ());

	p.SetCamera(cam->GetPos(), cam->GetTarget(), cam->GetUp());
	p.SetPerspectiveProj(60.0f, 800, 600, 1.0f, 100.0f);	// 45.0 is a good value

	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans());
	glUniform4f(mycolor, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f);	// color changer

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

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

Cube::Cube(const Point3 & _center) : GameObject(_center, 2, 2, 2) {
	numOfTriangles = 12;
	//vertex buffer
	cout << "started creating cube"<<endl;

	Vector3f Vertices[8];
    Vertices[0] = Vector3f(-1.0f, -1.0f, 1.0f);
    Vertices[1] = Vector3f(1.0f, -1.0f, 1.0f);
    Vertices[2] = Vector3f(1.0f, 1.0f, 1.0);
    Vertices[3] = Vector3f(-1.0f, 1.0f, 1.0f);

	Vertices[4] = Vector3f(-1.0f, -1.0f, -1.0f);
	Vertices[5] = Vector3f(1.0f, -1.0f, -1.0f);
	Vertices[6] = Vector3f(1.0f, 1.0f, -1.0f);
	Vertices[7] = Vector3f(-1.0f, 1.0f, -1.0f);

	cout << "assigned vectors" << endl;

 	glGenBuffers(1, &VBO);

	cout <<"genBuffers finished" << endl;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	cout << "bind buffer finished " << endl;
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);



	cout << "VErtex created" << endl;

	//element buffer
	unsigned int Indices[] = { 0, 1, 2,
                               2, 3, 0,
                               3, 2, 6,
                               6, 7, 3,
								7, 6, 5,
								5, 4, 7,
								4,5,1,
								1,0,4,
								4,0,3,	
								3,7,4,
								1,5,6,
								6,2,1};

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	cout << "elements created" << endl;


	CompileShaders();

	cout << "Compiled shaders" << endl;
}

HorizontalPlane::HorizontalPlane(const Point3 & c, float x, float y, float z)
: GameObject(c, x, y, z) {

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

	/*Vertices[0] = Vector3f(-2.0f, -2.0f, 2.0f);
	Vertices[1] = Vector3f(2.0f, -2.0f, 2.0f);
	Vertices[2] = Vector3f(2.0f, 2.0f, 2.0);
	Vertices[3] = Vector3f(-2.0f, 2.0f, 2.0f);

	Vertices[4] = Vector3f(-2.0f, -2.0f, -2.0f);
	Vertices[5] = Vector3f(2.0f, -2.0f, -2.0f);
	Vertices[6] = Vector3f(2.0f, 2.0f, -2.0f);
	Vertices[7] = Vector3f(-2.0f, 2.0f, -2.0f);*/

	cout << "assigned vectors" << endl;

	glGenBuffers(1, &VBO);

	cout << "genBuffers finished" << endl;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	cout << "bind buffer finished " << endl;
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);



	cout << "VErtex PLANCE created" << endl;

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

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	cout << "elements created" << endl;


	CompileShaders();
}


bool HorizontalPlane::collidesWith(const BoundingBox & box) {
	Rect plane = { (int)(center.getX()-halfDistX.getX()), (int)(center.getZ()-halfDistZ.getZ()), (int)(2*halfDistX.getX()), (int)(2*halfDistZ.getZ()) };
	Point2 p = { (int)box.center.getX(), (int)box.center.getZ() };

	// further check the whole plane around center point

	return center.getY() > box.center.getY() - box.halfDistY.getY() - 0.05
		&& center.getY() < box.center.getY() - box.halfDistY.getY() + 0.05
		&& plane.contains(p);
}
