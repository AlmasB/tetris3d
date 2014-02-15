#include "GraphicsEngine.h"

GraphicsEngine::GraphicsEngine() : rot(0.0f) {}

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
		SDL_WINDOW_OPENGL);	// shown?

	if (window == NULL)
		return _SDL_ERROR_INIT_WINDOW + std::string(SDL_GetError());

	glContext = SDL_GL_CreateContext(window);

	if (glContext == NULL)
		return _SDL_ERROR_INIT_OPENGL + std::string(SDL_GetError());






	int oglIdx = -1;
	int nRD = SDL_GetNumRenderDrivers();
	for (int i = 0; i<nRD; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
		{
			if (!strcmp(info.name, "opengl"))
			{
				oglIdx = i;
			}
		}
	}


	renderer = SDL_CreateRenderer(window, oglIdx, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);






	initGL();

	SDL_SetRelativeMouseMode(SDL_TRUE);	// trap mouse inside for fps mode

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

/*void GraphicsEngine::drawCube(GLfloat x, GLfloat y, GLfloat z) {
	RGB color = { 128, 128, 128 };
	drawCube(x, y, z, color);
}

void GraphicsEngine::drawCube(GLfloat x, GLfloat y, GLfloat z, RGB color) {

	//glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
	glPushMatrix();
	// Render a color-cube consisting of 6 quads with different colors
	//glLoadIdentity();                 // Reset the model-view matrix
	glTranslatef(x, y, z);  // Move right and into the screen
	//glColor3i(color.r, color.g, color.b);
	//glColor3f(0.5f, 0.5f, 0.5f);
	glColor3f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);

	//glRotatef(30.0f, 0.0f, 1.0f, 0.0f);   // rotate

	//rot += 0.5f;



	glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
	// Top face (y = 1.0f)
	// Define vertices in counter-clockwise (CCW) order with normal pointing out
	//glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// Bottom face (y = -1.0f)
	//glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Front face  (z = 1.0f)
	//glColor3f(1.0f, 0.7f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Back face (z = -1.0f)
	//glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	// Left face (x = -1.0f)
	//glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Right face (x = 1.0f)
	//glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();  // End of drawing color-cube

	glPopMatrix();
}*/

void GraphicsEngine::clearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
}

void GraphicsEngine::showScreen() {
	//SDL_GL_SwapWindow(window);
	SDL_RenderPresent(renderer);
}

void GraphicsEngine::drawUI() {
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-400, 400, 300, -300, 0, 1);
	glColor4f(0, 0, 1.0f, 1.0f);

	// need to specify renderer ?
	//SDL_SetRenderDrawColor(renderer, 255, 10, 10, 255);
	SDL_RenderDrawLine(renderer, 5, 0, 5+10, +0);
	SDL_RenderDrawLine(renderer, -5, 0, -5-10, +0);
	SDL_RenderDrawLine(renderer, 0, 0, +0, +10);
	SDL_RenderDrawLine(renderer, 0, 0, +0, -10);
	glPopMatrix();
}