#ifndef ENGINECORE_HPP
#define ENGINECORE_HPP

#include "Input.hpp"
#include "common.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdexcept>
#include <sstream>
#include <iostream>

struct SDL_Window;
struct SDL_Renderer;


class EngineCore
{
public:
	EngineCore();
	virtual ~EngineCore();

    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;

    void executeLoop();
    void render();
    void eventHandling(Input& input);


private:
	bool mainLoopQuit = false;

	EngineCore(const EngineCore& other);
	EngineCore& operator=(const EngineCore& other);



};

#endif /* end of include guard: ENGINECORE_HPP */
