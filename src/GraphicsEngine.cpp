#include "GraphicsEngine.h"

GraphicsEngine::GraphicsEngine() : fpsAverage(0), fpsPrevious(0), fpsStart(0), fpsEnd(0) {}

GraphicsEngine::~GraphicsEngine() {
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

std::string GraphicsEngine::init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return _SDL_ERROR_INIT + std::string(SDL_GetError());

	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	window = SDL_CreateWindow(_ENGINE_TITLE,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		__ENGINE_WINDOW_W, __ENGINE_WINDOW_H,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);	// shown?

	if (window == NULL)
		return _SDL_ERROR_INIT_WINDOW + std::string(SDL_GetError());

	glContext = SDL_GL_CreateContext(window);

	if (glContext == NULL)
		return _SDL_ERROR_INIT_OPENGL + std::string(SDL_GetError());

	glEnable(GL_DEPTH_TEST);	// when do we do that? before/after
	glDepthFunc(GL_LESS);

	if (glewInit() != GLEW_OK)
		return _GL_ERROR_GLEW;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    /*glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f); // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);	// not sure if need them
	//glDepthFunc(GL_LESS);
	glDepthFunc(GL_LEQUAL);*/

	/*int oglIdx = -1;
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
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);*/

	SDL_GL_SetSwapInterval(1);

	SDL_SetRelativeMouseMode(SDL_TRUE);	// trap mouse inside for fps mode

	return _ENGINE_ERROR_NONE;
}

void GraphicsEngine::initGL() {
}

void GraphicsEngine::resize() {

}

void GraphicsEngine::setWindowTitle(const char * title) {
	string t = string(title) + string(_ENGINE_TITLE);	// check for memory
	SDL_SetWindowTitle(window, t.c_str());
}

void GraphicsEngine::clearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsEngine::showScreen() {
	SDL_GL_SwapWindow(window);
	//SDL_RenderPresent(renderer);	// use SDL_GL directly if renderer won't work for 2D
}

void GraphicsEngine::drawUI() {
	/*SDL_SetRenderDrawColor(renderer, 255, 10, 10, 255);
	SDL_RenderDrawLine(renderer, 5, 0, 5 + 10, +0);
	SDL_RenderDrawLine(renderer, -5, 0, -5 - 10, +0);
	SDL_RenderDrawLine(renderer, 0, 0, +0, +10);
	SDL_RenderDrawLine(renderer, 0, 0, +0, -10);*/


	/*glPushMatrix();
	glLoadIdentity();
	glOrtho(-400, 400, 300, -300, 0, 1);
	glColor4f(0, 0, 1.0f, 1.0f);

	// need to specify renderer ?
	//SDL_SetRenderDrawColor(renderer, 255, 10, 10, 255);
	SDL_RenderDrawLine(renderer, 5, 0, 5+10, +0);
	SDL_RenderDrawLine(renderer, -5, 0, -5-10, +0);
	SDL_RenderDrawLine(renderer, 0, 0, +0, +10);
	SDL_RenderDrawLine(renderer, 0, 0, +0, -10);
	glPopMatrix();*/
}

/*void GraphicsEngine::setWindowIcon(const char *iconFileName) {
	SDL_Surface * icon = IMG_Load(iconFileName);
	SDL_SetWindowIcon(window, icon);
	SDL_FreeSurface(icon);
}*/

void GraphicsEngine::setWindowSize(const int &w, const int &h) {
	SDL_SetWindowSize(window, w, h);
}

void GraphicsEngine::setFrameStart() {
	fpsStart = SDL_GetTicks();
}

void GraphicsEngine::adjustFPSDelay(const Uint32 &delay) {
	fpsEnd = SDL_GetTicks() - fpsStart;
	if (fpsEnd < delay) {
		SDL_Delay(delay - fpsEnd);
	}

	Uint32 fpsCurrent = 1000 / (SDL_GetTicks() - fpsStart);
	fpsAverage = (fpsCurrent + fpsPrevious + fpsAverage * 8) / 10;	// average, 10 values / 10
	fpsPrevious = fpsCurrent;
}

Uint32 GraphicsEngine::getAverageFPS() {
	return fpsAverage;
}
