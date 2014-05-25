#include "Game.h"
#include "../test/TestGame.h"

int mainImpl() {
	try {
		//Game game;
		TestGame game;
		return game.runMainLoop();
	}
	catch (int code) {
		std::cout << "Game launch failed. See above trace (if any). Press Enter to exit" << std::endl;
		getchar();
		return code;
	}
}

int main(int argc, char * args[]) {
	return mainImpl();
}