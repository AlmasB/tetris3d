#include "Game.h"

#include "../engine/graphics/MD3Loader.h"

int main(int argc, char * args[]) {

	std::shared_ptr<Game> game = std::shared_ptr<Game>(new Game());
	if (!game->init()) {
		std::cout << "Game init failed. See above trace (if any). Press Enter to exit" << std::endl;
		getchar();
		return -1;
	}

	game->runMainLoop();
	game.reset();

#ifdef __DEBUG
	debug("The game finished and cleaned up successfully. Press Enter to exit");
	getchar();
#endif

	

	/*MD3Loader loader;

	loader.loadModel("res/upper.md3");
	getchar();*/

	return 0;
}