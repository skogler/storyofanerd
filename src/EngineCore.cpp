
#include "EngineCore.hpp"

#define FPS 50

EngineCore::EngineCore() :
		mWindow(nullptr), mRenderer(nullptr) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		throw std::runtime_error("Could not initialize the SDL2 library!");
	}

	mWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
	if (!mWindow) {
		std::stringstream ss("Error opening window: ");
		ss << SDL_GetError();
		throw std::runtime_error(ss.str());
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer) {
		std::stringstream ss("Error creating renderer: ");
		ss << SDL_GetError();
		throw std::runtime_error(ss.str());
	}

	executeLoop();
}

EngineCore::~EngineCore() {
	if (mWindow) {
		SDL_DestroyWindow(mWindow);
	}
	SDL_Quit();
}

///////////////////////////////////////////////////////////////////////////

void EngineCore::executeLoop() {
	SDL_Event sdl_event;
	Input input;

	//FPS variables
	int sdl_last_tick = 0;
	int sdl_fps_intervall = 1000 / FPS;

	while (!mainLoopQuit) {
		render();
		eventHandling(input);

		//FPS code
		if(sdl_last_tick + sdl_fps_intervall > SDL_GetTicks()){
			SDL_Delay((sdl_last_tick + sdl_fps_intervall) - SDL_GetTicks() );
		}
		sdl_last_tick = SDL_GetTicks();

	}
}
///////////////////////////////////////////////////////////////////////////

void EngineCore::render() {
	SDL_RenderPresent(mRenderer);
}

///////////////////////////////////////////////////////////////////////////

void EngineCore::eventHandling(Input& input) {
    auto& actionList = input.getActions();
    for (auto& action : actionList) {
        if (action == InputAction::EXIT) {
            mainLoopQuit = true;
        }
    }
}

