#include <iostream>

#include "GraphicsEngine.h"
#include "Box.h"
#include "Camera.h"

using namespace std;	// for easier debugging, will be removed

/*
// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, ly = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, y = 0.0f, z = 0.0f;

float fraction = 0.2f;
*/

static const int UP = 0;
static const int DOWN = 1;
static const int LEFT = 2;
static const int RIGHT = 3;

bool keys[4];

bool running = true;

Camera * camera;

void handleKeyEvent(const SDL_Keycode &key, bool keyDown) {
	//if (keyDown) {
		switch (key) {
			case SDLK_RIGHT: case SDLK_d:
				keys[RIGHT] = keyDown;
				break;
			case SDLK_LEFT: case SDLK_a:
				keys[LEFT] = keyDown;
				break;
			case SDLK_UP: case SDLK_w:
				keys[UP] = keyDown;
				break;
			case SDLK_DOWN: case SDLK_s:
				keys[DOWN] = keyDown;
				break;
			case SDLK_ESCAPE:
				running = false;
				break;
		}
	//}
}

void onKeyUP() {
	camera->moveForward();
	//x += lx * fraction;
	//z += lz * fraction;
}

void onKeyDown() {
	camera->moveBack();
	//x -= lx * fraction;
	//z -= lz * fraction;
}

void onKeyLeft() {
	camera->moveLeft();
	//x -= -lz * fraction;
	//z -= lx * fraction;
}

void onKeyRight() {
	camera->moveRight();
	//x += -lz * fraction;
	//z += lx * fraction;
}

void onMouseUp(int v) {
	camera->rotateUp(v * 0.0035f);
	//ly += 0.05f;
}

void onMouseDown() {
	camera->rotateDown(0.04f);
	//ly -= 0.05f;
}

void onMouseLeft(int v) {
	//camera->rotateLeft(0.05f);
	//angle -= 0.05f;	// TODO: pick right angle/speed/checking for FPS mode
	//lx = sin(angle);
	//lz = -cos(angle);

}

void onMouseRight(int v) {
	camera->rotateRight(v * 0.0035f);
	//angle += 0.05f;
	//lx = sin(angle);
	//lz = -cos(angle);

}

int main(int argc, char * args[]) {

	camera = new Camera();

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

	
	SDL_Event event;
	Uint32 start, end;

	GLdouble eyeX = 0, eyeY = 0, eyeZ = 5.0;
	GLdouble toX = 0, toY = 0, toZ = 0;

	Box * b1 = new Box(5, 3, -7);
	Box * b2 = new Box(0, 0, -10);
	Box * b3 = new Box(0, 6, -4);

	Vector3 gravity(0, -0.01f, 0);	// should be vector really

	SDL_SetRelativeMouseMode(SDL_TRUE);	// trap mouse inside for fps mode

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
	


		int mx, my;
		SDL_GetRelativeMouseState(&mx, &my);	// like normal window coord positive down and right

		onMouseRight(mx);
		onMouseUp(-my);
		//onMouseLeft(mx);
		/*if (mx > 2.5) {
			onMouseRight();
		}
		else if (mx < -2.5) {
			onMouseLeft();
		}

		if (my < -2.5) {
			onMouseUp();
		}
		else if (my > 2.5) {
			onMouseDown();
		}*/




		if (keys[UP])
			onKeyUP();
		if (keys[DOWN])
			onKeyDown();
		if (keys[RIGHT])
			onKeyRight();
		if (keys[LEFT])
			onKeyLeft();

		// update

		if (b1->center.getY() > 0)
			b1->center += gravity;
		if (b2->center.getY() > 0)
			b2->center += gravity;
		if (b3->center.getY() > 0)
			b3->center += gravity;
		

		// render
		gfx->clearScreen();


		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		// Set the camera
		/*gluLookAt(x, y, z,
			x + lx, y + ly, z + lz,
			0.0f, 1.0f, 0.0f);*/

		Point3 pos = camera->getPosition();
		Point3 look = pos + camera->getDirection();

		Vector3 up = camera->getUP();

		gluLookAt(pos.getX(), pos.getY(), pos.getZ(),
			look.getX(), look.getY(), look.getZ(),
			up.getX(), up.getY(), up.getZ());

		// some plane underneath
		glColor3f(1.0f, 0.9f, 1.0f);

		glBegin(GL_QUADS);
		glVertex3f(-100.0f, -1.0f, -100.0f);
		glVertex3f(-100.0f, -1.0f, 100.0f);
		glVertex3f(100.0f, -1.0f, 100.0f);
		glVertex3f(100.0f, -1.0f, -100.0f);
		glEnd();
		
		glColor3f(0.5f, 0.5f, 0.2f);
		// get some cubes up
		b1->draw();
		b2->draw();
		b3->draw();

		gfx->drawUI();

		gfx->showScreen();

		end = SDL_GetTicks() - start;
		if (end < 20) {
			SDL_Delay(20 - end);
		}
	}

	delete b1;
	delete b2;
	delete b3;

	delete gfx;
	return 0;
}