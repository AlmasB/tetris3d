#include "Game.h"

int main(int argc, char * args[]) {

	Game * game = new Game();
	if (!game->init())
		return -1;

	game->runMainLoop();
	delete game;

	return 0;
}