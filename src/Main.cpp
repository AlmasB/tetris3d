#include <iostream>

#include "GraphicsEngine.h"

/*void handleKeys(unsigned char key, int x, int y)
{
	if (key == 'q')
	{
	}

}*/

// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, z = 5.0f;

float fraction = 0.2;

void handleKeyEvent(const SDL_Keycode &key, bool keyDown) {
	if (keyDown) {
		switch (key) {
			case SDLK_RIGHT:
				//std::cout << "right" << std::endl;
				angle += 0.05f;
				lx = sin(angle);
				lz = -cos(angle);
				break;
			case SDLK_LEFT:
				//std::cout << "left" << std::endl;
				angle -= 0.05f;
				lx = sin(angle);
				lz = -cos(angle);

				break;
			case SDLK_UP:
				x += lx * fraction;
				z += lz * fraction;
				break;
			case SDLK_DOWN:
				x -= lx * fraction;
				z -= lz * fraction;
				//std::cout << eyeZ << std::endl;
				break;
		}
	}
}

int main(int argc, char * args[]) {

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

			if (event.type == SDL_QUIT) {
				running = false;
			}
			else if (event.type == SDL_KEYDOWN) {
				//if (event.key.repeat == 1) {
					handleKeyEvent(event.key.keysym.sym, event.type == SDL_KEYDOWN);
				//}
			}

			/*else if (e.type == SDL_TEXTINPUT)
			{
				int x = 0, y = 0;
				SDL_GetMouseState(&x, &y);
				handleKeys(e.text.text[0], x, y);
			}*/
		}
	

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


		glColor3f(0.9f, 0.9f, 0.9f);
		//glLoadIdentity();
		//glTranslatef(0, -1.0, 0);
		/*glBegin(GL_QUADS);

		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f, 100.0f);
		glVertex3f(100.0f, 0.0f, 100.0f);
		glVertex3f(100.0f, 0.0f, -100.0f);
		glEnd();
		*/
		//glTranslatef(0, 1, 0);
		gfx->drawCube(3, 0, -5);
		gfx->drawCube(-7, 0, 6);
		gfx->drawCube(0, 0, -15);
		gfx->drawCube(-3, 3, 0);

		//glTranslatef(0, 0, 0);

		gfx->showScreen();

		end = SDL_GetTicks() - start;
		if (end < 20) {
			SDL_Delay(20 - end);
		}
	}

	delete gfx;
	return 0;
}





/*int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Main loop flag
		bool quit = false;

		//Event handler
		SDL_Event e;

		//Enable text input
		SDL_StartTextInput();

		Uint32 start, end;

		GLfloat dist = 0.0f;

		//While application is running
		while (!quit)
		{
			start = SDL_GetTicks();
			//std::cout << start << std::endl;

			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				int x, y;
				if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
					rotquad += 0.5f;
				}

				//User requests quit
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				//Handle keypress with current mouse position
				else if (e.type == SDL_TEXTINPUT)
				{
					int x = 0, y = 0;
					SDL_GetMouseState(&x, &y);
					handleKeys(e.text.text[0], x, y);
				}
			}

			// update cube
			rotquad += 1.0f;

			//Render quad
			display(0.0f, 0.0f, -10.0f);

			//Update screen
			SDL_GL_SwapWindow(gWindow);

			end = SDL_GetTicks() - start;
			if (end < 20) {
				SDL_Delay(20 - end);
			}
		}

		//Disable text input
		SDL_StopTextInput();
	}

	//Free resources and close SDL
	close();

	return 0;
}*/
