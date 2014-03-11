#include "Player.h"

Player::Player(const Point3f & center) 
: GameObject(center, 1.5f, 2.0f, 1.5f, SDL_COLOR_GRAY), score(0), lives(3) {}

void Player::addScore(int value) {
	score += value;
#ifdef __DEBUG
	debug("Score:", score);
#endif
}

void Player::addLives(int value) {
	lives += value;
#ifdef __DEBUG
	debug("Lives:", lives);
#endif
}