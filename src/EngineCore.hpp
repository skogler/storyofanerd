#ifndef ENGINECORE_HPP
#define ENGINECORE_HPP

#include "Input.hpp"
#include "Audio.hpp"
#include "common.h"
#include "Player.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdexcept>
#include <sstream>
#include <iostream>

#define FPS 50
#define TILE_W 20
#define TILE_H 20
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

struct SDL_Window;
struct SDL_Renderer;

class EngineCore {
public:
	EngineCore();
	virtual ~EngineCore();

	void executeLoop();
	void render();
	void eventHandling(Input& input);

	//World offset
	int xoffset = 0;
	int yoffset = 0;

private:

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Player* player;

	bool mainLoopQuit = false;

	EngineCore(const EngineCore& other);
	EngineCore& operator=(const EngineCore& other);

protected:
    std::unique_ptr<Audio> mAudio;
};

#endif /* end of include guard: ENGINECORE_HPP */
