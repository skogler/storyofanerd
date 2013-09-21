#include "EngineCore.hpp"

#include "common.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdexcept>
#include <sstream>

EngineCore::EngineCore()
    : mWindow(nullptr)
    , mRenderer(nullptr)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        throw std::runtime_error("Could not initialize the SDL2 library!");
    }
    mWindow = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (!mWindow){
        std::stringstream ss("Error opening window: ");
        ss << SDL_GetError();
        throw std::runtime_error(ss.str());
    }
}

EngineCore::~EngineCore()
{
    if (mWindow) {
        SDL_DestroyWindow(mWindow);
    }
	SDL_Quit();
}
