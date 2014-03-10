#include "Game.h"

int main(int argc, char * args[]) {

	std::unique_ptr<Game> game;
	
	try {
		game = std::unique_ptr<Game>(new Game());
	}
	catch (int exit) {
		std::cout << "Game init failed. See above trace (if any). Press Enter to exit" << std::endl;
		getchar();
		return exit;
	}

	game->runMainLoop();
	game.reset();

#ifdef __DEBUG
	debug("The game finished and cleaned up successfully. Press Enter to exit");
	getchar();
#endif

	return 0;
}