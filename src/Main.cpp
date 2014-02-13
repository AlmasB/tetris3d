#include <iostream>

#include "GraphicsEngine.h"

/*void handleKeys(unsigned char key, int x, int y)
{
	if (key == 'q')
	{
	}

}*/

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

	GLfloat x = 1.0f, y = 0.0f, z = -10.0f;

	while (running) {
		start = SDL_GetTicks();

		while (SDL_PollEvent(&event) != 0) {

			if (event.type == SDL_QUIT) {
				running = false;
			}
			/*else if (e.type == SDL_TEXTINPUT)
			{
				int x = 0, y = 0;
				SDL_GetMouseState(&x, &y);
				handleKeys(e.text.text[0], x, y);
			}*/
		}
	

		// update

		// render
		gfx->clearScreen();
		gfx->drawCube(x, y, z);
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
