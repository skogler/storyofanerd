#include "Player.h"

#include "xmlloader.h"
#include "Audio.hpp"

#include <sstream>

Player::Player(const std::shared_ptr<LoadedMap>& map,
		const std::shared_ptr<Audio>& audio, const SDL_Rect& viewport) :
		mMap(map), mAudio(audio), mViewport(viewport), mSpeed(0.2f), mJumpDuration(
				400), mJumping(false), mMovementState(
				PlayerMovementState::STANDING) {
	mBoundingBox.w = 20;
	mBoundingBox.h = 20;
	mBoundingBox.x = mViewport.w / 2 - mBoundingBox.w / 2;
	//mBoundingBox.y = mViewport.h / 2 - mBoundingBox.h / 2;
	mBoundingBox.y = 0;
	mMovementState = PlayerMovementState::STANDING;
}

Player::~Player() {
}

void Player::update(int delta) {
	mOldBoundingBox = mBoundingBox;
	if (mJumping) {
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

	if (mMovementState == PlayerMovementState::RUNNING_LEFT) {
		mBoundingBox.x -= static_cast<int>(2 * mSpeed * delta);
	} else if (mMovementState == PlayerMovementState::RUNNING_RIGHT) {
		mBoundingBox.x += static_cast<int>(2 * mSpeed * delta);
	}
	mMovementState = PlayerMovementState::STANDING;

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
				mJumping = false;
				//mPlayerImage = animations[PlayerMovementState::STANDING]; //TODO: falling?

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
	mPlayerImage = animations[PlayerMovementState::RUNNING_LEFT];
}

void Player::moveRight() {
	mMovementState = PlayerMovementState::RUNNING_RIGHT;
	mPlayerImage = animations[PlayerMovementState::RUNNING_RIGHT];
}

void Player::jump() {
	mJumping = true;
	mPlayerImage = animations[PlayerMovementState::JUMPING];
}

const SDL_Rect& Player::getBoundingBox() {
	return mBoundingBox;
}

SDL_Texture* Player::getPlayerImage() {
	return mPlayerImage;
}

void Player::loadAnimations(SDL_Renderer* mRenderer) {
	//TODO: rework.... urgent...
	SDL_Texture* anim_tex;

	anim_tex = IMG_LoadTexture(mRenderer, "../res/animations/jumping.png");
	animations[PlayerMovementState::JUMPING] = &(*anim_tex);
	anim_tex = IMG_LoadTexture(mRenderer, "../res/animations/moveLeft.png");
	animations[PlayerMovementState::RUNNING_LEFT] = &(*anim_tex);
	anim_tex = IMG_LoadTexture(mRenderer, "../res/animations/standing.png");
	animations[PlayerMovementState::STANDING] = &(*anim_tex);
	anim_tex = IMG_LoadTexture(mRenderer, "../res/animations/standing2.png");
	animations[PlayerMovementState::STANDING2] = &(*anim_tex);
	anim_tex = IMG_LoadTexture(mRenderer, "../res/animations/moveRight.png");
	animations[PlayerMovementState::RUNNING_RIGHT] = &(*anim_tex);
	/*
	 SDL_Texture *anim_tex1;
	 anim_tex1 = IMG_LoadTexture(mRenderer, "../res/animations/jumping.png");
	 animations[PlayerMovementState::JUMPING] = anim_tex1;
	 SDL_Texture *anim_tex2;
	 anim_tex2 = IMG_LoadTexture(mRenderer, "../res/animations/moveLeft.png");
	 animations[PlayerMovementState::RUNNING_LEFT] = anim_tex2;
	 SDL_Texture *anim_tex3;
	 anim_tex3 = IMG_LoadTexture(mRenderer, "../res/animations/standing.png");
	 animations[PlayerMovementState::STANDING] = anim_tex3;
	 SDL_Texture *anim_tex4;
	 anim_tex4 = IMG_LoadTexture(mRenderer, "../res/animations/standing2.png");
	 animations[PlayerMovementState::STANDING2] = anim_tex4;
	 SDL_Texture *anim_tex5;
	 anim_tex5 = IMG_LoadTexture(mRenderer, "../res/animations/moveRight.png");
	 animations[PlayerMovementState::RUNNING_RIGHT] = anim_tex5;

	 */

	mPlayerImage = animations[PlayerMovementState::STANDING];

}
