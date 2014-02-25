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

	window = SDL_CreateWindow(_ENGINE_TITLE,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		__ENGINE_WINDOW_W, __ENGINE_WINDOW_H,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);	// shown?

	if (nullptr == window)
		return _SDL_ERROR_INIT_WINDOW + std::string(SDL_GetError());

	glContext = SDL_GL_CreateContext(window);

	if (nullptr == glContext)
		return _SDL_ERROR_INIT_OPENGL + std::string(SDL_GetError());

	glEnable(GL_DEPTH_TEST);	// TODO: when do we do that? before/after
	glDepthFunc(GL_LESS);

	if (glewInit() != GLEW_OK)
		return _GL_ERROR_GLEW;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/*glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f); // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);	// not sure if need them
	glDepthFunc(GL_LESS);
	glDepthFunc(GL_LEQUAL);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);	// TODO: check documentation
	glEnable(GL_CULL_FACE);*/

	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetSwapInterval(1);

	SDL_SetRelativeMouseMode(SDL_TRUE);	// trap mouse inside for fps mode

	return _ENGINE_ERROR_NONE;
}

void GraphicsEngine::initGL() {
}

void GraphicsEngine::setWindowTitle(const char * title) {
	std::string t = std::string(title) + std::string(_ENGINE_TITLE);	// check for memory
	SDL_SetWindowTitle(window, t.c_str());
}

void GraphicsEngine::clearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsEngine::showScreen() {
	SDL_GL_SwapWindow(window);
}

void GraphicsEngine::drawUI(uint score, uint lives) {
	// convert uints to string/char*
	// surfaceUI = TTF_RenderTextSolid
	// create opengl texture 
	// pass to ui object ?
}

/*void GraphicsEngine::setWindowIcon(const char *iconFileName) {
	SDL_Surface * icon = IMG_Load(iconFileName);
	SDL_SetWindowIcon(window, icon);
	SDL_FreeSurface(icon);
}*/

GLuint GraphicsEngine::createGLTextureFromSurface(SDL_Surface * surface) {
	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);


	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);

	//glBindTexture(GL_TEXTURE_2D, 0);	// unbind

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

/* CAMERA CLASS DEFINITION BEGIN */

Camera * Camera::instance = nullptr;

Camera::Camera() : Movable(), center(Point3f(0, 0, 0)) {
	assigned = nullptr;
	cameraPerspective.fov = 60.0f;
	cameraPerspective.width = __ENGINE_WINDOW_W * 1.0f;
	cameraPerspective.height = __ENGINE_WINDOW_H * 1.0f;
	cameraPerspective.zNear = 1.0f;
	cameraPerspective.zFar = 100.0f;
}

Camera* Camera::getInstance() {
	if (instance == nullptr)
		instance = new Camera();
	return instance;
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

	Camera * camera = Camera::getInstance();

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
