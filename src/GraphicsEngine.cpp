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

	glEnable(GL_DEPTH_TEST);	// TODO: when do we do that? before/after
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
	std::string t = std::string(title) + std::string(_ENGINE_TITLE);	// check for memory
	SDL_SetWindowTitle(window, t.c_str());
}

void GraphicsEngine::clearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsEngine::showScreen() {
	SDL_GL_SwapWindow(window);
	//SDL_RenderPresent(renderer);	// use SDL_GL directly if renderer won't work for 2D
}

GLuint GraphicsEngine::loadTexture() {

	GLuint texture;

	//Load the image from the file into SDL's surface representation
	SDL_Surface* surface = IMG_Load("texture2.png");
	if (surface == NULL) { //If it failed, say why and don't continue loading the texture
		printf("Error: \"%s\"\n", SDL_GetError()); return 25;
	}

	//Generate an array of textures.  We only want one texture (one element array), so trick
	//it by treating "texture" as array of length one.
	glGenTextures(1, &texture);
	//Select (bind) the texture we just generated as the current 2D texture OpenGL is using/modifying.
	//All subsequent changes to OpenGL's texturing state for 2D textures will affect this texture.
	glBindTexture(GL_TEXTURE_2D, texture);
	//Specify the texture's data.  This function is a bit tricky, and it's hard to find helpful documentation.  A summary:
	//   GL_TEXTURE_2D:    The currently bound 2D texture (i.e. the one we just made)
	//               0:    The mipmap level.  0, since we want to update the base level mipmap image (i.e., the image itself,
	//                         not cached smaller copies)
	//         GL_RGBA:    The internal format of the texture.  This is how OpenGL will store the texture internally (kinda)--
	//                         it's essentially the texture's type.
	//      surface->w:    The width of the texture
	//      surface->h:    The height of the texture
	//               0:    The border.  Don't worry about this if you're just starting.
	//          GL_RGB:    The format that the *data* is in--NOT the texture!  Our test image doesn't have an alpha channel,
	//                         so this must be RGB.
	//GL_UNSIGNED_BYTE:    The type the data is in.  In SDL, the data is stored as an array of bytes, with each channel
	//                         getting one byte.  This is fairly typical--it means that the image can store, for each channel,
	//                         any value that fits in one byte (so 0 through 255).  These values are to interpreted as
	//                         *unsigned* values (since 0x00 should be dark and 0xFF shold be bright).
	// surface->pixels:    The actual data.  As above, SDL's array of bytes.
	
	
	
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	
	
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	
	
	
	
	
	
	//Set the minification and magnification filters.  In this case, when the texture is minified (i.e., the texture's pixels (texels) are
	//*smaller* than the screen pixels you're seeing them on, linearly filter them (i.e. blend them together).  This blends four texels for
	//each sample--which is not very much.  Mipmapping can give better results.  Find a texturing tutorial that discusses these issues
	//further.  Conversely, when the texture is magnified (i.e., the texture's texels are *larger* than the screen pixels you're seeing
	//them on), linearly filter them.  Qualitatively, this causes "blown up" (overmagnified) textures to look blurry instead of blocky.
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Unload SDL's copy of the data; we don't need it anymore because OpenGL now stores it in the texture.
	SDL_FreeSurface(surface);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	return texture;
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
