#include "Player.h"

Player::Player()
    : mSpeed(0.3f)
    , mJumpDuration(400)
    , mJumping(false)
    , mMovementState(PlayerMovementState::STANDING)
{
	mBoundingBox.x = 320;
	mBoundingBox.y = 200;
	mBoundingBox.w = 40;
	mBoundingBox.h = 40;
}

Player::~Player()
{
}

void Player::update(int delta)
{
    if (mJumping) {
        if (mJumpElapsedTime < mJumpDuration) {
            mJumpElapsedTime += delta;
            mBoundingBox.y -= static_cast<int>(delta * mSpeed * 2);
        }
    }

    if (mMovementState == PlayerMovementState::RUNNING_LEFT) {
        mBoundingBox.x -= static_cast<int>(2 * mSpeed * delta);
        // Clamp
        if (mBoundingBox.x <= 0) {
            mBoundingBox.x = 0;
            mMovementState = PlayerMovementState::STANDING;
        }
    } else if (mMovementState == PlayerMovementState::RUNNING_RIGHT) {
        mBoundingBox.x += static_cast<int>(2 * mSpeed * delta);
    }

    // Fall
	if((mBoundingBox.y + mBoundingBox.h) < 440) {
		mBoundingBox.y += static_cast<int>(mSpeed * delta);
    } else if (mJumping) {
        // Reset jump variables once we are back on the ground
        mJumpElapsedTime = 0;
        mJumping = false;
    }
    mMovementState = PlayerMovementState::STANDING;
}

void Player::moveLeft()
{
    mMovementState = PlayerMovementState::RUNNING_LEFT;
}

void Player::moveRight()
{
    mMovementState = PlayerMovementState::RUNNING_RIGHT;
}

void Player::jump()
{
    mJumping = true;
}

const SDL_Rect& Player::getBoundingBox()
{
	return mBoundingBox;
}
