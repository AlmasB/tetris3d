#include "GraphicsEngine.h"

GraphicsEngine::GraphicsEngine() {}
GraphicsEngine::~GraphicsEngine() {
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

std::string GraphicsEngine::init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return _SDL_ERROR_INIT + std::string(SDL_GetError());

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	window = SDL_CreateWindow(_ENGINE_TITLE,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		__ENGINE_WINDOW_W, __ENGINE_WINDOW_H,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (window == NULL)
		return _SDL_ERROR_INIT_WINDOW + std::string(SDL_GetError());

	glContext = SDL_GL_CreateContext(window);

	if (glContext == NULL)
		return _SDL_ERROR_INIT_OPENGL + std::string(SDL_GetError());

	initGL();

	return _ENGINE_ERROR_NONE;
}

void GraphicsEngine::initGL() {
	SDL_GL_SetSwapInterval(1);

	resize();

	/*****************************************************************************
	* Taken from LazyFoo tutorials for proper OpenGL init
	*
	****************************************************************************/

	// TODO: can anything from below fail ? gluErrorString(glGetError()) ?

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

/*****************************************************************************
 * Taken from LazyFoo tutorials for opengl viewport resize if there's change
 * in window size
 *
 ****************************************************************************/
void GraphicsEngine::resize() {
	GLfloat aspect = (GLfloat)__ENGINE_WINDOW_W / (GLfloat)__ENGINE_WINDOW_H;

	// Set the viewport to cover the new window
	glViewport(0, 0, __ENGINE_WINDOW_W, __ENGINE_WINDOW_H);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
	// Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void GraphicsEngine::drawCube(GLfloat &x, GLfloat &y, GLfloat &z) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

	// Render a color-cube consisting of 6 quads with different colors
	glLoadIdentity();                 // Reset the model-view matrix
	glTranslatef(x, y, z);  // Move right and into the screen
	//glRotatef(rotquad, 0.0f, 1.0f, 0.0f);   // rotate

	glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
	// Top face (y = 1.0f)
	// Define vertices in counter-clockwise (CCW) order with normal pointing out
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// Bottom face (y = -1.0f)
	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Front face  (z = 1.0f)
	glColor3f(1.0f, 0.7f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Back face (z = -1.0f)
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	// Left face (x = -1.0f)
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Right face (x = 1.0f)
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();  // End of drawing color-cube
}