#ifndef __GFX_ENGINE_H__
#define __GFX_ENGINE_H__

#include <string>
#include <memory>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <GL/glew.h>
#include <GL/glu.h>

#include "EngineCommon.h"
#include "Movable.h"

/* ENGINE DEFAULT SETTINGS */

#define _ENGINE_TITLE     "Powered by T.W.O. Game Engine, by Almas"
#define __ENGINE_WINDOW_W 800
#define __ENGINE_WINDOW_H 600

static const SDL_Color SDL_COLOR_GRAY = { 0x80, 0x80, 0x80 };
static const SDL_Color SDL_COLOR_YELLOW = {0xFF, 0xFF, 0};
static const SDL_Color SDL_COLOR_RED = { 0xFF, 0, 0 };
static const SDL_Color SDL_COLOR_GREEN = { 0, 0xFF, 0 };
static const SDL_Color SDL_COLOR_BLUE = { 0, 0, 0xFF };

struct Perspective {
	float fov;	// field of view
	float width;
	float height;
	float zNear;
	float zFar;
};

class Camera : public Movable {
	friend class CameraTransformer;
	friend class GraphicsEngine;
	private:

		//static std::shared_ptr<Camera> instance;

		Point3f center;
		static Camera * instance;
		Perspective cameraPerspective;

		std::shared_ptr<Movable> assigned;

		Camera();
	public:
		void updateView();
		//static std::shared_ptr<Camera> getInstance();
		static Camera * getInstance();
		void move(const Vector3f & v);
		Point3f getCenter();


		void follow(std::shared_ptr<Movable> objectToFollow);
};

class CameraTransformer {
	private:
		Matrix4f transformation;
	public:
		CameraTransformer(Point3f center);
		Vector3f scale;
		Vector3f center;
		Vector3f rotate;
		const Matrix4f* transform();

		void printDebug();
};

class GraphicsEngine {
	friend class GameEngine;
	private:
		SDL_Window * window;
		SDL_GLContext glContext;

		TTF_Font * font;

		Uint32 fpsAverage, fpsPrevious, fpsStart, fpsEnd;

		
		GraphicsEngine();

	public:
		
		~GraphicsEngine();
		std::string init();
		void initGL();

		SDL_Surface * surfaceUI;

		/**
		* Creates a GL 2D texture from SDL_Surface* and returns its id
		* This function does NOT free the surface passed in
		*
		* @pre-condition: surface must be valid, i.e. != nullptr
		*
		* @return created texture id, if errors occurred or surf is invalid - 0
		*/
		static GLuint createGLTextureFromSurface(SDL_Surface * surf);

		void useFont(TTF_Font * font);
		GLuint createGLTextureFromText(std::string text, SDL_Color textColor);

		void clearScreen();
		void showScreen();

		void setWindowIcon(const char *);
		void setWindowSize(const int &, const int &);
		void setWindowTitle(const char *title);

		void setFrameStart();
		void adjustFPSDelay(const Uint32 &);
		Uint32 getAverageFPS();
};

typedef GraphicsEngine GFX;

#endif
