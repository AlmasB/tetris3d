#include "GraphicsEngine.h"

GraphicsEngine::GraphicsEngine() : fpsAverage(0), fpsPrevious(0), fpsStart(0), fpsEnd(0) {}

GraphicsEngine::~GraphicsEngine() {
#ifdef __DEBUG
	debug("GraphicsEngine::~GraphicsEngine() started");
#endif

	IMG_Quit();
	TTF_Quit();
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();

#ifdef __DEBUG
	debug("GraphicsEngine::~GraphicsEngine() finished");
#endif
}

std::string GraphicsEngine::init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return "Failed to init SDL: " + std::string(SDL_GetError());

	window = SDL_CreateWindow(_ENGINE_TITLE,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		__ENGINE_WINDOW_W, __ENGINE_WINDOW_H,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (nullptr == window)
		return "Failed to create SDL_Window: " + std::string(SDL_GetError());

	glContext = SDL_GL_CreateContext(window);

	if (nullptr == glContext)
		return "Failed to init OpenGL context: " + std::string(SDL_GetError());

	// although not necessary, SDL doc says to prevent hiccups load it before using
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		return "Failed to init SDL_image: " + std::string(IMG_GetError());
	}

	if (TTF_Init() < 0)
		return "Failed to init SDL_ttf: " + std::string(TTF_GetError());

	if (glewInit() != GLEW_OK)
		return "Failed to init GLEW";

	initGL();

	return _ENGINE_ERROR_NONE;
}

void GraphicsEngine::initGL() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	SDL_GL_SetSwapInterval(0);	// on NVIDIA drivers setting to 1 causes high cpu load
}

void GraphicsEngine::setWindowTitle(const char * title) {
	std::string t = std::string(title) + std::string(_ENGINE_TITLE);
	SDL_SetWindowTitle(window, t.c_str());
}

void GraphicsEngine::clearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsEngine::showScreen() {
	SDL_GL_SwapWindow(window);
}

void GraphicsEngine::useFont(TTF_Font * _font) {
	if (nullptr != font) {
		TTF_CloseFont(font);
	}
	font = _font;
}

void GraphicsEngine::setWindowIcon(const char *iconFileName) {
	SDL_Surface * icon = IMG_Load(iconFileName);
	SDL_SetWindowIcon(window, icon);
	SDL_FreeSurface(icon);
}

GLuint GraphicsEngine::createGLTextureFromText(std::string text, SDL_Color color) {
	// blend is supposed to be much nicer when no need for fast swapping
	SDL_Surface * textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
	SDL_Surface * background = IMG_Load("res/white128.png");

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

void GraphicsEngine::setWindowSize(const int &w, const int &h) {
	SDL_SetWindowSize(window, w, h);
	Camera::instance->cameraPerspective.width = w * 1.0f;
	Camera::instance->cameraPerspective.width = h * 1.0f;
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

Camera::Camera() : Movable(), center(Point3f(0, 0, 0)) {
	assigned = nullptr;
	cameraPerspective.fov = 60.0f;
	cameraPerspective.width = __ENGINE_WINDOW_W * 1.0f;
	cameraPerspective.height = __ENGINE_WINDOW_H * 1.0f;
	cameraPerspective.zNear = 1.0f;
	cameraPerspective.zFar = 100.0f;
}

void Camera::follow(std::shared_ptr<Movable> objectToFollow) {
	assigned = objectToFollow;
}

void Camera::updateView() {
	if (assigned != nullptr) {
		center = assigned->getCenter();
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

CameraTransformer::CameraTransformer(Point3f _center)
: scale(Vector3f(1.0f, 1.0f, 1.0f)), center(Vector3f(_center.x, _center.y, _center.z)), rotate(Vector3f(0, 0, 0)) {

}

const Matrix4f * CameraTransformer::transform() {
	Matrix4f scaleTrans, rotateTrans, translationTrans, cameraTranslationTrans, cameraRotateTrans, persProjTrans;

	// scale, rotate, translate
	scaleTrans.initScaleTransform(scale.x, scale.y, scale.z);
	rotateTrans.initRotateTransform(rotate.x, rotate.y, rotate.z);
	translationTrans.initTranslationTransform(center.x, center.y, center.z);

	std::shared_ptr<Camera> camera = Camera::instance;

	// camera transformations
	cameraTranslationTrans.initTranslationTransform(-camera->getCenter().x, -camera->getCenter().y, -camera->getCenter().z);
	cameraRotateTrans.initCameraTransform(camera->getDirection(), camera->getUpVector());	// careful passing references
	persProjTrans.initPersProjTransform(camera->cameraPerspective.fov, camera->cameraPerspective.width, camera->cameraPerspective.height, camera->cameraPerspective.zNear, camera->cameraPerspective.zFar);

	transformation = persProjTrans * cameraRotateTrans * cameraTranslationTrans * translationTrans * rotateTrans * scaleTrans;


	return &transformation;
	//return trans;
}

void CameraTransformer::printDebug() {
	std::cout << center.x << " " << center.y << " " << center.z << std::endl;
}
