#ifndef __GFX_ENGINE_H__
#define __GFX_ENGINE_H__

#include <string>
#include <memory>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifdef _WIN32
	#include <SDL2/SDL_ttf.h>
#else
	#include <SDL/SDL_ttf.h>
#endif

#include <GL/glew.h>
#include <GL/glu.h>

#include "../core/EngineCommon.h"
#include "Movable.h"
#include "ShaderManager.h"

/* ENGINE DEFAULT SETTINGS */

#define _ENGINE_TITLE     "Powered by The Cube Game Engine, by Almas"

static const int DEFAULT_WINDOW_WIDTH = 800;
static const int DEFAULT_WINDOW_HEIGHT = 600;

static const SDL_Color SDL_COLOR_GRAY = { 0x80, 0x80, 0x80 };
static const SDL_Color SDL_COLOR_YELLOW = {0xFF, 0xFF, 0};
static const SDL_Color SDL_COLOR_RED = { 0xFF, 0, 0 };
static const SDL_Color SDL_COLOR_GREEN = { 0, 0xFF, 0 };
static const SDL_Color SDL_COLOR_BLUE = { 0, 0, 0xFF };

inline SDL_Color getRandomColor(int minRGB, int maxRGB) {
	SDL_Color color = { (Uint8)getRandom(minRGB, maxRGB), (Uint8)getRandom(minRGB, maxRGB), (Uint8)getRandom(minRGB, maxRGB) };
	return color;
}

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
		Point3f center;
		Perspective cameraPerspective;

		std::shared_ptr<Movable> assigned;
		static std::shared_ptr<Camera> instance;
		Camera();
	public:
		void updateView();
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
};

class GraphicsEngine {
	friend class GameEngine;
	private:
		SDL_Window * window;
		SDL_GLContext glContext;

		TTF_Font * font;

		SDL_Surface * textureBackground;

		Uint32 fpsAverage, fpsPrevious, fpsStart, fpsEnd;

		GraphicsEngine();
		void initGL();

	public:	
		~GraphicsEngine();

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

		/**
		* Call this method after you have finished drawing
		*/
		void showScreen();

		/**
		 * Draws a GL Texture in a 2D screen space
		 */
		void drawGLTexture(GLuint textureID, float x, float y, int w, int h);

		void drawText(std::string text, SDL_Color color, float x, float y);
		
		/**
		* Draws SDL_Surface on screen starting at x, y
		*
		* Provides similar functionality to <em>deprecated</em> glOrtho in that
		* it draws in a 2D screen space
		*
		* @pre-condition: surface must be valid, i.e. != nullptr
		*/
		void drawSDLSurface(SDL_Surface * surf, float x, float y);
		void drawSDLSurface(SDL_Surface * surf, float x, float y, int w, int h);

		void setWindowIcon(const char *);
		void setWindowSize(const int &, const int &);
		void setWindowTitle(const char *title);
		void setFullscreen(bool);

		Dimension2i getWindowSize();

		void setFrameStart();
		void adjustFPSDelay(const Uint32 &);
		Uint32 getAverageFPS();

		std::shared_ptr<Camera> getCamera();

		
};

typedef GraphicsEngine GFX;

#endif
