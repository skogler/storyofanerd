
#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Player {
public:
	Player();
	virtual ~Player();

	void moveLeft();
	void moveRight();
	void jump();
	void fall();
	void changeDirection();

	SDL_Rect getBoundingBox();
	SDL_Surface* getPlayerImage();

private:
	SDL_Rect bounding_box;
	SDL_Surface* player_image;

	// for sync
	int lastActionTick;



};

#endif /* PLAYER_H_ */
