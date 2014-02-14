#include <iostream>

#include "GraphicsEngine.h"

// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, z = 5.0f;

float fraction = 0.2;

static const int UP = 0;
static const int DOWN = 1;
static const int LEFT = 2;
static const int RIGHT = 3;

bool keys[4];

void handleKeyEvent(const SDL_Keycode &key, bool keyDown) {
	//if (keyDown) {
		switch (key) {
			case SDLK_RIGHT:
				keys[RIGHT] = keyDown;
				break;
			case SDLK_LEFT:
				keys[LEFT] = keyDown;

				break;
			case SDLK_UP:
				keys[UP] = keyDown;
				break;
			case SDLK_DOWN:
				keys[DOWN] = keyDown;
				break;
		}
	//}
}

void onKeyUP() {
	x += lx * fraction;
	z += lz * fraction;
}

void onKeyDown() {
	x -= lx * fraction;
	z -= lz * fraction;
}

void onKeyLeft() {
	angle -= 0.05f;
	lx = sin(angle);
	lz = -cos(angle);
}

void onKeyRight() {
	angle += 0.05f;
	lx = sin(angle);
	lz = -cos(angle);
}

int main(int argc, char * args[]) {

	keys[UP] = false;
	keys[DOWN] = false;
	keys[LEFT] = false;
	keys[RIGHT] = false;

	GraphicsEngine * gfx = new GraphicsEngine();
	std::string error;
	if ((error = gfx->init()) != _ENGINE_ERROR_NONE) {
		std::cout << error << std::endl;
		getchar();
		return -1;
	}

	bool running = true;
	SDL_Event event;
	Uint32 start, end;

	GLdouble eyeX = 0, eyeY = 0, eyeZ = 5.0;
	GLdouble toX = 0, toY = 0, toZ = 0;

	while (running) {
		start = SDL_GetTicks();

		while (SDL_PollEvent(&event) != 0) {
			switch (event.type) {
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:	//FALLTHRU
				case SDL_KEYUP:
					if (event.key.repeat == 0) {
						handleKeyEvent(event.key.keysym.sym, event.type == SDL_KEYDOWN);
					}
					break;
			}
		}
	
		if (keys[UP])
			onKeyUP();
		if (keys[DOWN])
			onKeyDown();
		if (keys[RIGHT])
			onKeyRight();
		if (keys[LEFT])
			onKeyLeft();

		// update

		//gfx->resize();

		

		// render
		gfx->clearScreen();


		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		// Set the camera
		gluLookAt(x, 1.0f, z,
			x + lx, 1.0f, z + lz,
			0.0f, 1.0f, 0.0f);

		// some plane underneath
		glColor3f(1.0f, 0.9f, 1.0f);

		glBegin(GL_QUADS);
		glVertex3f(-100.0f, -1.0f, -100.0f);
		glVertex3f(-100.0f, -1.0f, 100.0f);
		glVertex3f(100.0f, -1.0f, 100.0f);
		glVertex3f(100.0f, -1.0f, -100.0f);
		glEnd();
		
		// get some cubes up
		gfx->drawCube(3, 0, -5);
		gfx->drawCube(-7, 0, 6);
		gfx->drawCube(0, 0, -15);
		gfx->drawCube(-3, 3, 0);
		gfx->drawCube(-2, 4, -3);

		gfx->showScreen();

		end = SDL_GetTicks() - start;
		if (end < 20) {
			SDL_Delay(20 - end);
		}
	}

	delete gfx;
	return 0;
}