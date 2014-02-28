#include "Game.h"
#include "Level.h"

int main(int argc, char * args[]) {

	std::shared_ptr<Game> game = std::shared_ptr<Game>(new Game());
	if (!game->init()) {
		std::cout << "Game init failed. See above trace (if any). Press any key to exit" << std::endl;
		getchar();
		return -1;
	}

	game->runMainLoop();
	game.reset();

#ifdef __DEBUG
	debug("The game finished and cleaned up successfully. Press any key to exit");
	getchar();
#endif

	return 0;
}