#include "Player.h"

Player::Player() {

	lastActionTick = SDL_GetTicks();
	bounding_box.x = 320;
	bounding_box.y = 440;
	bounding_box.w = 40;
	bounding_box.h = 40;

}

Player::~Player() {
	//SDL_FreeSurface(player_image);
}

void Player::moveLeft() {
	bounding_box.x -= 5;
	changeDirection();

}

void Player::moveRight() {
	bounding_box.x += 5;
	changeDirection();
}

void Player::jump() {
	if(bounding_box.y > 1)
		bounding_box.y -= 60;
}

void Player::fall() {
	if(bounding_box.y < 440)
		bounding_box.y += 1;
}
void Player::changeDirection() {
	// change picture
}

SDL_Rect Player::getBoundingBox() {
	return bounding_box;
}

SDL_Surface* Player::getPlayerImage() {
	return player_image;
}
