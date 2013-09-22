#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>
#include <map>

enum class PlayerMovementState
{
	JUMPING,
	JUMPING_LEFT,
	JUMPING_RIGHT,
    RUNNING_LEFT,
    RUNNING_RIGHT,
    STANDING,
    STANDING2
};

class Audio;
class LoadedMap;

class Player
{
public:
	Player(const std::shared_ptr<LoadedMap>& map, const std::shared_ptr<Audio>& audio, const SDL_Rect& viewport);
	virtual ~Player();

    void update(int delta);
	void moveLeft();
	void moveRight();
	void jump();

	const SDL_Rect& getBoundingBox();
    SDL_Texture* getPlayerImage();
	inline void setBoundingBox(SDL_Rect rect) {
		mBoundingBox = rect;
	}
	void addAnimation(PlayerMovementState ps,SDL_Texture animation);
	void loadAnimations(SDL_Renderer* mRenderer);

protected:
    std::shared_ptr<LoadedMap> mMap;
    std::shared_ptr<Audio> mAudio;
    const SDL_Rect& mViewport;

    const float mSpeed;
    const unsigned int mJumpDuration;
    unsigned int mStandDuration;
    PlayerMovementState mMovementState;
    SDL_Texture* mPlayerImage;
    std::map<PlayerMovementState,SDL_Texture*> animations;

	SDL_Rect mBoundingBox;
    SDL_Rect mOldBoundingBox;
    int mJumpElapsedTime;
    bool mStandingOnFloor;

    void animate();


};

#endif /* PLAYER_H_ */
