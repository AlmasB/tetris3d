#include "GameObject.h"

GameObject::GameObject(const Point3 & _center, float x, float y, float z, RGBColor _color)
	: BoundingBox(_center, x, y, z), color(_color) {
	alive = true;
	locked = false;
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

Cube::Cube(const Point3 & _center, int color) : center(_center) {






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


	CompileShaders(color);

	cout << "Compiled shaders" << endl;



}





void Cube::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }

    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0]= strlen(pShaderText);
    glShaderSource(ShaderObj, 1, p, Lengths);
    glCompileShader(ShaderObj);
    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

    glAttachShader(ShaderProgram, ShaderObj);
}

void Cube::CompileShaders(int color)
{

	// color 0 - blue
		// 1 - red

    GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

	cout << color << endl;

    AddShader(ShaderProgram, pVS, GL_VERTEX_SHADER);
    AddShader(ShaderProgram, color == 0 ? pFS : pFS2, GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        	exit(1);
	}

    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glUseProgram(ShaderProgram);

    gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");
    assert(gWVPLocation != 0xFFFFFFFF);
}









void Cube::draw(std::shared_ptr<Camera> cam) {


	    

    static float Scale = 0.0f;

    Scale += 0.25f;

    Pipeline p;
    //p.Rotate(0.0f, Scale, 0.0f);
    //p.WorldPos(0.0f, 0.0f, 7.0f);
    p.WorldPos(center.getX(), center.getY(), center.getZ());

    p.SetCamera(cam->GetPos(), cam->GetTarget(), cam->GetUp());
    p.SetPerspectiveProj(60.0f, 800, 600, 1.0f, 100.0f);

    glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans());

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glDrawElements(GL_TRIANGLES, 3*12, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);










	/*glPushMatrix();
	glTranslatef(center.getX(), center.getY(), center.getZ());

	if (locked){
		glColor3f(COLOR_YELLOW.r / 255.0f, COLOR_YELLOW.g / 255.0f, COLOR_YELLOW.b / 255.0f);
	}
	else {
		glColor3f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
	}

	glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
	// Top face (y = 1.0f)
	// Define vertices in counter-clockwise (CCW) order with normal pointing out
	glVertex3f(halfDistX.getX(), halfDistY.getY(), -halfDistZ.getZ());
	glVertex3f(-halfDistX.getX(), halfDistY.getY(), -halfDistZ.getZ());
	glVertex3f(-halfDistX.getX(), halfDistY.getY(), halfDistZ.getZ());
	glVertex3f(halfDistX.getX(), halfDistY.getY(), halfDistZ.getZ());

	// Bottom face (y = -1.0f)
	glVertex3f(halfDistX.getX(), -halfDistY.getY(), halfDistZ.getZ());
	glVertex3f(-halfDistX.getX(), -halfDistY.getY(), halfDistZ.getZ());
	glVertex3f(-halfDistX.getX(), -halfDistY.getY(), -halfDistZ.getZ());
	glVertex3f(halfDistX.getX(), -halfDistY.getY(), -halfDistZ.getZ());

	// Front face  (z = 1.0f)
	glVertex3f(halfDistX.getX(), halfDistY.getY(), halfDistZ.getZ());
	glVertex3f(-halfDistX.getX(), halfDistY.getY(), halfDistZ.getZ());
	glVertex3f(-halfDistX.getX(), -halfDistY.getY(), halfDistZ.getZ());
	glVertex3f(halfDistX.getX(), -halfDistY.getY(), halfDistZ.getZ());

	// Back face (z = -1.0f)
	glVertex3f(halfDistX.getX(), -halfDistY.getY(), -halfDistZ.getZ());
	glVertex3f(-halfDistX.getX(), -halfDistY.getY(), -halfDistZ.getZ());
	glVertex3f(-halfDistX.getX(), halfDistY.getY(), -halfDistZ.getZ());
	glVertex3f(halfDistX.getX(), halfDistY.getY(), -halfDistZ.getZ());

	// Left face (x = -1.0f)
	glVertex3f(-halfDistX.getX(), halfDistY.getY(), halfDistZ.getZ());
	glVertex3f(-halfDistX.getX(), halfDistY.getY(), -halfDistZ.getZ());
	glVertex3f(-halfDistX.getX(), -halfDistY.getY(), -halfDistZ.getZ());
	glVertex3f(-halfDistX.getX(), -halfDistY.getY(), halfDistZ.getZ());

	// Right face (x = 1.0f)
	glVertex3f(halfDistX.getX(), halfDistY.getY(), -halfDistZ.getZ());
	glVertex3f(halfDistX.getX(), halfDistY.getY(), halfDistZ.getZ());
	glVertex3f(halfDistX.getX(), -halfDistY.getY(), halfDistZ.getZ());
	glVertex3f(halfDistX.getX(), -halfDistY.getY(), -halfDistZ.getZ());
	glEnd();

	glPopMatrix();*/
}

HorizontalPlane::HorizontalPlane(const Point3 & c, float x, float y, float z, RGBColor _color)
: GameObject(c, x, y, z, _color) {

}

void HorizontalPlane::draw() {
	/*glPushMatrix();
	glTranslatef(center.getX(), center.getY(), center.getZ());

	glColor3f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);

	glBegin(GL_QUADS);
	glVertex3f(-halfDistX.getX(), 0.0f, -halfDistZ.getZ());	// forget about Y for now
	glVertex3f(-halfDistX.getX(), 0.0f, halfDistZ.getZ());
	glVertex3f(halfDistX.getX(), 0.0f, halfDistZ.getZ());
	glVertex3f(halfDistX.getX(), 0.0f, -halfDistZ.getZ());
	glEnd();

	glPopMatrix();*/
}

bool HorizontalPlane::collidesWith(const BoundingBox & box) {
	Rect plane = { (int)(center.getX()-halfDistX.getX()), (int)(center.getZ()-halfDistZ.getZ()), (int)(2*halfDistX.getX()), (int)(2*halfDistZ.getZ()) };
	Point2 p = { (int)box.center.getX(), (int)box.center.getZ() };

	// further check the whole plane around center point

	return center.getY() > box.center.getY() - box.halfDistY.getY() - 0.05
		&& center.getY() < box.center.getY() - box.halfDistY.getY() + 0.05
		&& plane.contains(p);
}
