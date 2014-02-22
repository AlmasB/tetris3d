#include "Player.h"

Player::Player(const Point3f & _center) : Cube(_center, 2.0f, COLOR_GOLD), score(0), lives(3) {

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
