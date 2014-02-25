#include "Player.h"

Player::Player(const Point3f & _center) : GameObject(_center, 2.0f, 2.0f, 2.0f, SDL_COLOR_GRAY), score(0), lives(3) {

}

void Player::addScore(uint value) {
	score += value;
	std::cout << "Score: " << score << std::endl;
}

void Player::addLives(int value) {
	lives += value;
	std::cout << "Lives: ";
	for (uint i = 0; i < lives; ++i)
		std::cout << "* ";
	std::cout << std::endl;
}

uint Player::getLives() {
	return lives;
}

uint Player::getScore() {
	return score;
}
