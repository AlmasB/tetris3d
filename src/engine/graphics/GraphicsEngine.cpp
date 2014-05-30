#include "GraphicsEngine.h"

GraphicsEngine::GraphicsEngine() : fpsAverage(0), fpsPrevious(0), fpsStart(0), fpsEnd(0) {
	window = SDL_CreateWindow(DEFAULT_WINDOW_TITLE.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (nullptr == window)
		throw EngineException("Failed to create window", SDL_GetError());

	glContext = SDL_GL_CreateContext(window);

	if (nullptr == glContext)
		throw EngineException("Failed to create OpenGL context", SDL_GetError());

	// although not necessary, SDL doc says to prevent hiccups load it before using
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
		throw EngineException("Failed to init SDL_image - PNG", IMG_GetError());

	if (TTF_Init() < 0)
		throw EngineException("Failed to init SDL_ttf", TTF_GetError());

	if (glewInit() != GLEW_OK)
		throw EngineException("Failed to init GLEW");

	initGL();

	textureBackground = IMG_Load("res/trans128.png");
	if (nullptr == textureBackground)
		throw EngineException("Failed to load texture background", IMG_GetError());
}

GraphicsEngine::~GraphicsEngine() {
#ifdef __DEBUG
	debug("GraphicsEngine::~GraphicsEngine() started");
#endif

	SDL_FreeSurface(textureBackground);
	IMG_Quit();
	TTF_Quit();
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();

#ifdef __DEBUG
	debug("GraphicsEngine::~GraphicsEngine() finished");
#endif
}

void GraphicsEngine::initGL() {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	if (SDL_GL_SetSwapInterval(0) < 0) {	// on NVIDIA drivers setting to 1 causes high cpu load
#ifdef __DEBUG
		debug("Warning: SDL_GL_SetSwapInterval() mode isn't supported");
#endif
	}
}

void GraphicsEngine::setWindowTitle(const char * title) {
	SDL_SetWindowTitle(window, title);
#ifdef __DEBUG
	debug("Set window title to:", title);
#endif
}

void GraphicsEngine::setWindowTitle(const std::string & title) {
	SDL_SetWindowTitle(window, title.c_str());
#ifdef __DEBUG
	debug("Set window title to:", title.c_str());
#endif
}

void GraphicsEngine::setWindowIcon(const char *iconFileName) {
	SDL_Surface * icon = IMG_Load(iconFileName);
	if (nullptr == icon) {
		std::cout << "Failed to load icon: " << iconFileName << std::endl;
		std::cout << "Aborting: GraphicsEngine::setWindowIcon()" << std::endl;
		return;
	}
	SDL_SetWindowIcon(window, icon);
#ifdef __DEBUG
	debug("Set Window Icon to", iconFileName);
#endif
	SDL_FreeSurface(icon);
}

void GraphicsEngine::setFullscreen(bool b) {
	SDL_SetWindowFullscreen(window, b ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_MAXIMIZED);
}

void GraphicsEngine::setWindowSize(const int &w, const int &h) {
	SDL_SetWindowSize(window, w, h);
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
#ifdef __DEBUG
	debug("Set Window W", w);
	debug("Set Window H", h);
#endif
	Camera::instance->cameraPerspective.width = (float)w;
	Camera::instance->cameraPerspective.height = (float)h;
	glViewport(0, 0, w, h);	// Core in version 	4.4
}

Dimension2i GraphicsEngine::getWindowSize() {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	return Dimension2i(w, h);
}

Dimension2i GraphicsEngine::getMaximumWindowSize() {
	SDL_DisplayMode current;
	if (SDL_GetCurrentDisplayMode(0, &current) == 0) {
		return Dimension2i(current.w, current.h);
	}
	else {
		std::cout << "Failed to get window data" << std::endl;
		std::cout << "GraphicsEngine::getMaximumWindowSize() -> return (0, 0)" << std::endl;
		return Dimension2i(0, 0);
	}
}

void GraphicsEngine::clearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsEngine::showScreen() {
	SDL_GL_SwapWindow(window);
}

void GraphicsEngine::useFont(TTF_Font * _font) {
	if (nullptr == _font) {
#ifdef __DEBUG
		debug("GraphicsEngine::useFont()", "font is null");
#endif
		return;
	}

	font = _font;
}

GLuint GraphicsEngine::createGLTextureFromText(std::string text, SDL_Color color) {
	// blend is supposed to be much nicer when no need for fast swapping
	SDL_Surface * textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
	SDL_Surface * background = SDL_ConvertSurface(textureBackground, textureBackground->format, textureBackground->flags);

	SDL_BlitSurface(textSurface, 0, background, 0);

	GLuint textureID = createGLTextureFromSurface(background);
	SDL_FreeSurface(textSurface);
	SDL_FreeSurface(background);

	return textureID;
}

GLuint GraphicsEngine::createGLTextureFromSurface(SDL_Surface * surface) {
	if (nullptr == surface)	// just to be safe
		return 0;

	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);	// unbind

	return texture;
}

// NOT fully implemented
GLuint GraphicsEngine::createSkyboxGLTextureFromSurface(SDL_Surface * surface) {
	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	SDL_Surface * top = IMG_Load("res/skybox/top.png");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, top->w, top->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, top->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, top->w, top->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, top->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, top->w, top->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, top->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, top->w, top->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, top->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, top->w, top->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, top->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, top->w, top->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, top->pixels);

	// ... 5 more times

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);	// unbind

	std::cout << texture << std::endl;

	return texture;
}

// TODO: calculate higher power of two and use that instead of 128
void GraphicsEngine::drawText(std::string text, SDL_Color color, float x, float y) {
	// blend is supposed to be much nicer when no need for fast swapping
	// we ARE fast swapping now, should we change to different?
	SDL_Surface * textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
	SDL_Surface * background = SDL_ConvertSurface(textureBackground, textureBackground->format, textureBackground->flags);

	SDL_BlitSurface(textSurface, 0, background, 0);

	drawSDLSurface(background, x, y);

	SDL_FreeSurface(textSurface);
	SDL_FreeSurface(background);
}

void GraphicsEngine::drawSDLSurface(SDL_Surface * surface, float x, float y, int textureW, int textureH) {
	if (nullptr == surface)
		return;

	GLuint textureID = createGLTextureFromSurface(surface);
	drawGLTexture(textureID, x, y, textureW, textureH);

	glDeleteTextures(1, &textureID);
}

void GraphicsEngine::drawSDLSurface(SDL_Surface * surface, float x, float y) {
	if (nullptr == surface)
		return;

	drawSDLSurface(surface, x, y, surface->w, surface->h);
}

void GraphicsEngine::drawGLTexture(GLuint textureID, float x, float y, int textureW, int textureH) {
	if (0 == textureID)
		return;

	Dimension2i size = getWindowSize();
	float w = (float)size.w, h = (float)size.h;

	if (x > w || y > h)
		return;

	GLuint textureVBO;
	glGenBuffers(1, &textureVBO);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);

	GLfloat textureVertexData[] = {
		// X Y							U    V
		x, h - y - textureH, 0, 1.0f,
		x, h - y, 0, 0,
		x + textureW, h - y - textureH, 0.99f, 1.0f,

		x, h - y, 0, 0,
		x + textureW, h - y, 0.99f, 0,
		x + textureW, h - y - textureW, 0.99f, 1.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureVertexData), textureVertexData, GL_STATIC_DRAW);

	GLuint shaderProgram = ShaderManager::getInstance()->getCurrentProgram();

	GLuint useTextureLocation = glGetUniformLocation(shaderProgram, "useTexture");
	GLuint useUI = glGetUniformLocation(shaderProgram, "useUI");

	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(useTextureLocation, 1);

	glUniform1i(useUI, 1);

	// fast hack
	// TODO: make different shader prog for textures, or make autogenerated ones
	GLuint windowSizeLocation = glGetUniformLocation(shaderProgram, "windowSize");
	glUniform2f(windowSizeLocation, w / 2, h / 2);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);	// x,y repeat every 4 values

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 4 * sizeof(GLfloat), (const GLvoid*)(2 * sizeof(GLfloat)));	// u,v start at 2, repeat 4

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_TRIANGLES, 0, 6);	// 2*3

	glDisable(GL_BLEND);

	// unbind everything
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glUseProgram(0);

	glDeleteBuffers(1, &textureVBO);
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

std::shared_ptr<Camera> GraphicsEngine::getCamera() {
	return Camera::instance;
}

/* CAMERA CLASS DEFINITION BEGIN */

std::shared_ptr<Camera> Camera::instance = std::shared_ptr<Camera>(new Camera());

Camera::Camera() : Movable(), center(Point3f(0, 0, 0)), assigned(nullptr) {
	cameraPerspective.fov = 60.0f;
	cameraPerspective.width = (float) DEFAULT_WINDOW_WIDTH;
	cameraPerspective.height = (float) DEFAULT_WINDOW_HEIGHT;
	cameraPerspective.zNear = 1.0f;
	cameraPerspective.zFar = 1000.0f;
}

void Camera::follow(std::shared_ptr<Movable> objectToFollow) {
	assigned = objectToFollow;
}

void Camera::updateView() {
	if (assigned != nullptr) {
		center = assigned->getCenter() + Vector3f(0, 1.0f, 0);	// use object's "head", we prob need length in Y dir
		direction = assigned->getDirection();
		up = assigned->getUpVector();
	}
}

void Camera::move(const Vector3f& v) {
	center += v;
}

Point3f Camera::getCenter() {
	return center;
}

/* CAMERA TRANSFORMER CLASS DEFINITION BEGIN */

CameraTransformer::CameraTransformer(Point3f center)
: scale(Vector3f(1.0f, 1.0f, 1.0f)), center(Vector3f(center.x, center.y, center.z)), rotate(Vector3f(0, 0, 0)) {}

const Matrix4f * CameraTransformer::transform() {
	Matrix4f scaleTrans, rotateTrans, translationTrans, cameraTranslationTrans, cameraRotateTrans, persProjTrans;

	// scale, rotate, translate
	scaleTrans.scale(scale.x, scale.y, scale.z);
	rotateTrans.rotate(rotate.x, rotate.y, rotate.z);
	translationTrans.translate(center.x, center.y, center.z);

	std::shared_ptr<Camera> camera = Camera::instance;

	// camera transformations
	cameraTranslationTrans.translate(-camera->getCenter().x, -camera->getCenter().y, -camera->getCenter().z);
	cameraRotateTrans.initCameraTransform(camera->getDirection(), camera->getUpVector());
	persProjTrans.setPerspectiveProjection(camera->cameraPerspective.fov, camera->cameraPerspective.width, camera->cameraPerspective.height, camera->cameraPerspective.zNear, camera->cameraPerspective.zFar);

	transformation = persProjTrans * cameraRotateTrans * cameraTranslationTrans * translationTrans * rotateTrans * scaleTrans;

	return &transformation;
}
