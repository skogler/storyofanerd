#include "Player.h"

#include "xmlloader.h"
#include "Audio.hpp"

#include <sstream>

Player::Player(const std::shared_ptr<LoadedMap>& map,
		const std::shared_ptr<Audio>& audio, const SDL_Rect& viewport) :
		mMap(map), mAudio(audio), mViewport(viewport), mSpeed(0.2f), mStandDuration(
				0), mJumpDuration(400), mMovementState(
				PlayerMovementState::STANDING) {
	mBoundingBox.w = 20;
	mBoundingBox.h = 20;
	mBoundingBox.x = mViewport.w / 2 - mBoundingBox.w / 2;
	//mBoundingBox.y = mViewport.h / 2 - mBoundingBox.h / 2;
	mBoundingBox.y = 0;
}

Player::~Player() {
}

void Player::update(int delta) {
	animate();
	mOldBoundingBox = mBoundingBox;
	if (mMovementState == PlayerMovementState::JUMPING
			|| mMovementState == PlayerMovementState::JUMPING_LEFT
			|| mMovementState == PlayerMovementState::JUMPING_RIGHT) {
		if (mJumpElapsedTime == 0) {
			int index = random() % 3 + 1;
			std::stringstream filename;
			filename << "jump" << index << ".wav";
			mAudio->playSound(filename.str());
		}
		if (mJumpElapsedTime < mJumpDuration) {
			mJumpElapsedTime += delta;
			mBoundingBox.y -= static_cast<int>(delta * mSpeed * 2);
		}
	}

	if (mMovementState == PlayerMovementState::RUNNING_LEFT
			|| mMovementState == PlayerMovementState::JUMPING_LEFT) {
		mBoundingBox.x -= static_cast<int>(2 * mSpeed * delta);
	} else if (mMovementState == PlayerMovementState::RUNNING_RIGHT
			|| mMovementState == PlayerMovementState::JUMPING_RIGHT) {
		mBoundingBox.x += static_cast<int>(2 * mSpeed * delta);
	}

	mStandDuration += delta;
	if (mStandDuration <= (20 * delta)) {
		mMovementState = PlayerMovementState::STANDING2;
	} else if (mStandDuration <= (40 * delta)) {
		mMovementState = PlayerMovementState::STANDING;

	} else {
		mStandDuration = 0;
	}

	// Fall
	mBoundingBox.y += static_cast<int>(mSpeed * delta);

	// Check for collision
	for (auto collisionRect : mMap->getCollisionGeometry()) {
		//TODO optimize map translation
		collisionRect.y += mViewport.h
				- (mMap->getTileMap().height * mMap->getTileMap().tileheight);
		if (SDL_HasIntersection(&collisionRect, &mBoundingBox)) {
			SDL_Rect intersection;
			SDL_IntersectRect(&mBoundingBox, &collisionRect, &intersection);

			// If coming from above
			if ((mOldBoundingBox.y + mBoundingBox.h) <= collisionRect.y) {
				mBoundingBox.y = collisionRect.y - mBoundingBox.h;
				// Reset jump variables once we are back on the ground
				mJumpElapsedTime = 0;
			} else {
				// leave y as it is
				// except when coming from below
				if (mOldBoundingBox.y >= (collisionRect.y + collisionRect.h)) {
					mBoundingBox.y = (collisionRect.y + collisionRect.h);
				}
			}
			// If coming from left
			if ((mOldBoundingBox.x + mBoundingBox.w) <= collisionRect.x) {
				mBoundingBox.x = collisionRect.x - mBoundingBox.h;
			} else {
				// leave x as it is
				// except when coming from right
				if (mOldBoundingBox.x > (collisionRect.x + collisionRect.w)) {
					mBoundingBox.x = (collisionRect.x + collisionRect.w);
				}
			}
		}
	}
}

void Player::moveLeft() {
	mMovementState = PlayerMovementState::RUNNING_LEFT;
}

void Player::moveRight() {
	mMovementState = PlayerMovementState::RUNNING_RIGHT;
}

void Player::jump() {
	if (mMovementState == PlayerMovementState::RUNNING_RIGHT)
		mMovementState = PlayerMovementState::JUMPING_RIGHT;
	else if (mMovementState == PlayerMovementState::RUNNING_LEFT)
		mMovementState = PlayerMovementState::JUMPING_LEFT;
	else
		mMovementState = PlayerMovementState::JUMPING;

}

const SDL_Rect& Player::getBoundingBox() {
	return mBoundingBox;
}

SDL_Texture* Player::getPlayerImage() {
	return mPlayerImage;
}

void Player::animate() {
	switch (mMovementState) {
	case PlayerMovementState::JUMPING:
		mPlayerImage = animations[PlayerMovementState::JUMPING];
		break;
	case PlayerMovementState::JUMPING_RIGHT:
		mPlayerImage = animations[PlayerMovementState::JUMPING]; //TODO: add right animation
		break;
	case PlayerMovementState::JUMPING_LEFT:
		mPlayerImage = animations[PlayerMovementState::JUMPING]; //TODO: add left animation
		break;
	case PlayerMovementState::RUNNING_LEFT:
		mPlayerImage = animations[PlayerMovementState::RUNNING_LEFT];
		break;
	case PlayerMovementState::RUNNING_RIGHT:
		mPlayerImage = animations[PlayerMovementState::RUNNING_RIGHT];
		break;
	case PlayerMovementState::STANDING:
		mPlayerImage = animations[PlayerMovementState::STANDING];
		break;
	case PlayerMovementState::STANDING2:
		mPlayerImage = animations[PlayerMovementState::STANDING2];
		break;
	default:
		mPlayerImage = animations[PlayerMovementState::STANDING];

	}
}

void Player::loadAnimations(SDL_Renderer* mRenderer) {
	//TODO: rework.... urgent...
	SDL_Texture* anim_tex;
	anim_tex = IMG_LoadTexture(mRenderer, "../res/animations/jumping.png");
	animations[PlayerMovementState::JUMPING] = &(*anim_tex);
	anim_tex = IMG_LoadTexture(mRenderer, "../res/animations/moveLeft.png");
	animations[PlayerMovementState::RUNNING_LEFT] = &(*anim_tex);
	anim_tex = IMG_LoadTexture(mRenderer, "../res/animations/moveRight.png");
	animations[PlayerMovementState::RUNNING_RIGHT] = &(*anim_tex);
	anim_tex = IMG_LoadTexture(mRenderer, "../res/animations/standing.png");
	animations[PlayerMovementState::STANDING] = &(*anim_tex);
	anim_tex = IMG_LoadTexture(mRenderer, "../res/animations/standing2.png");
	animations[PlayerMovementState::STANDING2] = &(*anim_tex);

	mPlayerImage = animations[PlayerMovementState::STANDING];

}
