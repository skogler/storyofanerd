#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>

enum class PlayerMovementState
{
    RUNNING_LEFT,
    RUNNING_RIGHT,
    STANDING
};

class Audio;
class LoadedMap;

class Player
{
public:
	Player(const std::shared_ptr<LoadedMap>& map, const std::shared_ptr<Audio>& audio);
	virtual ~Player();

    void update(int delta);

	void moveLeft();
	void moveRight();
	void jump();

	const SDL_Rect& getBoundingBox();
	inline void setBoundingBox(SDL_Rect rect) {
		mBoundingBox = rect;
	}
	SDL_Surface* getPlayerImage();

protected:
    std::shared_ptr<LoadedMap> mMap;
    std::shared_ptr<Audio> mAudio;

    const float mSpeed;
    const unsigned int mJumpDuration;

    bool mJumping;
    PlayerMovementState mMovementState;

	SDL_Rect mBoundingBox;
    int mJumpElapsedTime;
};

#endif /* PLAYER_H_ */
