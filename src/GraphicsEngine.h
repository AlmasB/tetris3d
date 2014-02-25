#ifndef __GFX_ENGINE_H__
#define __GFX_ENGINE_H__

#include <string>
#include <memory>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <GL/glew.h>
#include <GL/glu.h>

#include "Movable.h"


/* STRING LITERALS - ERRORS */

#define _SDL_ERROR_INIT                 "Failed to init SDL"
#define _SDL_ERROR_INIT_WINDOW          "Failed to create window"
#define _SDL_ERROR_INIT_OPENGL		    "Failed to create OpenGL context"

#define _GL_ERROR_VSYNC					"Failed to set VSync"
#define _GL_ERROR_GLEW					"Failed to init GLEW"

#define _ENGINE_ERROR_NONE     ""
#define _ENGINE_ERROR_INIT_GFX "Failed to init graphics"
#define _ENGINE_ERROR_INIT_SFX "Failed to init audio"
#define _ENGINE_ERROR_RESOURCE "Failed to load resource "

/* ENGINE DEFAULT SETTING */

#define _ENGINE_TITLE     "Powered by T.W.O. Game Engine, by Almas"
#define __ENGINE_WINDOW_W 800
#define __ENGINE_WINDOW_H 600

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
	private:
		
		SDL_GLContext glContext;
		SDL_Renderer * renderer;

		Uint32 fpsAverage, fpsPrevious, fpsStart, fpsEnd;

	public:
		SDL_Window * window;
		GraphicsEngine();
		~GraphicsEngine();
		std::string init();
		void initGL();

		SDL_Surface * surfaceUI;

		void drawUI(uint score, uint lives);
		void clearScreen();
		void showScreen();

		//void setWindowIcon(const char *);
		void setWindowSize(const int &, const int &);
		void setWindowTitle(const char *title);

		void setFrameStart();
		void adjustFPSDelay(const Uint32 &);
		Uint32 getAverageFPS();
};


#endif
