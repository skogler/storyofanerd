
#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

enum class PlayerMovementState
{
    RUNNING_LEFT,
    RUNNING_RIGHT,
    STANDING
};

class Player
{
public:
	Player();
	virtual ~Player();

    void update(int delta);

	void moveLeft();
	void moveRight();
	void jump();

	const SDL_Rect& getBoundingBox();
	SDL_Surface* getPlayerImage();

protected:
    const float mSpeed;
    const unsigned int mJumpDuration;

    bool mJumping;
    PlayerMovementState mMovementState;

	SDL_Rect mBoundingBox;
    int mJumpElapsedTime;
};

#endif /* PLAYER_H_ */
