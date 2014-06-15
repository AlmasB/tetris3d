/**
 * {XCube3D Game Engine}
 * Copyright(C) { 2013-2014 }  {AlmasB} {a.baimagambetov1@uni.brighton.ac.uk}
 *
 * This program is free software; you can redistribute it and / or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110 - 1301 USA.
 */

#ifndef __GFX_ENGINE_H__
#define __GFX_ENGINE_H__

#include <string>
#include <memory>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <SDL2/SDL_ttf.h>

#include <GL/glew.h>

#if defined(__APPLE__)
	#include <OpenGL/glu.h>
#else
	#include <GL/glu.h>
#endif

#include "../core/EngineCommon.h"
#include "Movable.h"
#include "ShaderManager.h"

/* ENGINE DEFAULT SETTINGS */
static const std::string DEFAULT_WINDOW_TITLE = "The X-CUBE 3D Game Engine";
static const int DEFAULT_WINDOW_WIDTH = 800;
static const int DEFAULT_WINDOW_HEIGHT = 600;

static const SDL_Color SDL_COLOR_GRAY	= { 0x80, 0x80, 0x80 };
static const SDL_Color SDL_COLOR_YELLOW = { 0xFF, 0xFF, 0 };
static const SDL_Color SDL_COLOR_RED	= { 0xFF, 0, 0 };
static const SDL_Color SDL_COLOR_GREEN	= { 0, 0xFF, 0 };
static const SDL_Color SDL_COLOR_BLUE	= { 0, 0, 0xFF };
static const SDL_Color SDL_COLOR_BLACK = { 0, 0, 0 };
static const SDL_Color SDL_COLOR_WHITE = { 0xFF, 0xFF, 0xFF };
static const SDL_Color SDL_COLOR_AQUA = { 0, 0xFF, 0xFF };
static const SDL_Color SDL_COLOR_ORANGE = { 0xFF, 0xA5, 0 };
static const SDL_Color SDL_COLOR_PINK = { 0xFF, 0xC0, 0xCB };
static const SDL_Color SDL_COLOR_PURPLE = { 0x80, 0, 0x80 };
static const SDL_Color SDL_COLOR_VIOLET = { 0xEE, 0x82, 0xEE };

inline SDL_Color getRandomColor(int minRGB, int maxRGB) {
	SDL_Color color = { (Uint8)getRandom(minRGB, maxRGB), (Uint8)getRandom(minRGB, maxRGB), (Uint8)getRandom(minRGB, maxRGB) };
	return color;
}

// float pre-calculated values for GL drawing
struct SDL_Colorf {
	float r, g, b, a;
};

inline SDL_Colorf toSDLColorf(SDL_Color color) {
	SDL_Colorf colorf = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f };
	return colorf;
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

		static GLuint createSkyboxGLTextureFromSurface(SDL_Surface * surf);

		void useFont(TTF_Font * font);
		GLuint createGLTextureFromText(std::string text, SDL_Color textColor);

		/**
		* Clears everything on the screen
		* Call this before drawing anything to screen
		*/
		void clearScreen();

		/**
		* Displays everything rendered on the screen
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
		void setWindowTitle(const std::string &);
		void setFullscreen(bool);

		Dimension2i getWindowSize();
		/**
		* @return current display mode's resolution
		*         since most displays use native (max) resolution
		*         this returns maximum available (the one you can set) window size
		*/
		Dimension2i getMaximumWindowSize();

		void setFrameStart();
		void adjustFPSDelay(const Uint32 &);
		Uint32 getAverageFPS();

		std::shared_ptr<Camera> getCamera();	
};

typedef GraphicsEngine GFX;

#endif
