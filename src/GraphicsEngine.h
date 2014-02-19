#ifndef __GFX_ENGINE_H__
#define __GFX_ENGINE_H__

#include <string>

#include <SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>

#include "Camera.h"

#include <iostream>

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
		void resize();
		void drawUI();
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
