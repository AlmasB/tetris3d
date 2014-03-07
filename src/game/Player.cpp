#include "Player.h"

Player::Player(const Point3f & center) 
: GameObject(center, 1.5f, 2.0f, 1.5f, SDL_COLOR_GRAY), score(0), lives(3) {}

void Player::addScore(int value) {
	score += value;
#ifdef __DEBUG
	std::cout << "Score: " << score << std::endl;
#endif
}

void Player::addLives(int value) {
	lives += value;
#ifdef __DEBUG
	std::cout << "Lives: ";
	for (int i = 0; i < lives; ++i)
		std::cout << "* ";
	std::cout << std::endl;
#endif
}