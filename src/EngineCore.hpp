#ifndef ENGINECORE_HPP
#define ENGINECORE_HPP

#include "Input.hpp"
#include "Audio.hpp"
#include "common.h"
#include "Player.h"
#include "xmlloader.h"

#include "eventgen.h"

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

	void start();

private:
	EngineCore(const EngineCore& other);
	EngineCore& operator=(const EngineCore& other);

protected:
	void generateTilesetResources(int tileSetWidth, int tileSetHeight);
    void update(int delta);
	void render();
	void eventHandling(Input& input);

    // DATA
	bool mMainLoopQuit;
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
    SDL_Rect mViewport;
    SDL_Texture* bufferTex;
    std::unique_ptr<Player> mPlayer;
    std::shared_ptr<Audio> mAudio;

    std::shared_ptr<LoadedMap> mMap;
	SDL_Texture* tileSet;
	SDL_Texture* background;

	SDL_Rect backgroundRect;
	vector<SDL_Rect> tileClips;
	short current_clip = 0;

    Eventgen *mEventgen;
    EventhandlerMaster *mEventhandler;
};

#endif /* end of include guard: ENGINECORE_HPP */
