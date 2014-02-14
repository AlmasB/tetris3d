#ifndef __GAME_ENGINE_H__
#define __GAME_ENGINE_H__

#include <string>

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

/* STRING LITERALS - ERRORS */

#define _SDL_ERROR_INIT                 "Failed to init SDL"
#define _SDL_ERROR_INIT_WINDOW          "Failed to create window"
#define _SDL_ERROR_INIT_OPENGL		    "Failed to create OpenGL context"

#define _GL_ERROR_VSYNC					"Failed to set VSync"

#define _ENGINE_ERROR_NONE     ""
#define _ENGINE_ERROR_INIT_GFX "Failed to init graphics"
#define _ENGINE_ERROR_INIT_SFX "Failed to init audio"
#define _ENGINE_ERROR_RESOURCE "Failed to load resource "

/* ENGINE DEFAULT SETTING */

#define _ENGINE_TITLE     "Powered by T.W.O. Game Engine, by Almas"
#define __ENGINE_WINDOW_W 800
#define __ENGINE_WINDOW_H 600

// TODO: autocast to float, maybe even macro
struct RGB {
	int r, g, b;
};


class GraphicsEngine {
	private:
		SDL_Window * window;
		SDL_GLContext glContext;
		GLfloat rot;

	public:
		GraphicsEngine();
		~GraphicsEngine();
		std::string init();
		void initGL();
		void resize();
		void drawCube(GLfloat x, GLfloat y, GLfloat z);
		void drawCube(GLfloat x, GLfloat y, GLfloat z, RGB);
		void clearScreen();
		void showScreen();
};


#endif