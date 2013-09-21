#include "EngineCore.hpp"

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

	SDL_ShowCursor(SDL_DISABLE);

	player = new Player();
	map = new LoadedMap("../res/maps/testmap.tmx");

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
	Input input;
	//FPS variables
	int sdl_last_tick = 0;
	int sdl_fps_intervall = 1000 / FPS;

	while (!mainLoopQuit) {
		render();
		eventHandling(input);

		//FPS code
		if (sdl_last_tick + sdl_fps_intervall > SDL_GetTicks()) {
			SDL_Delay((sdl_last_tick + sdl_fps_intervall) - SDL_GetTicks());
		}
		sdl_last_tick = SDL_GetTicks();

	}
}
///////////////////////////////////////////////////////////////////////////

void EngineCore::render() {
	//Clear screen
	SDL_RenderSetScale(mRenderer, 1.f, 1.f);
	SDL_RenderClear(mRenderer);

	// tiles
	// map  00 01 02 03  | 11 12 ......
	SDL_Texture *tex = IMG_LoadTexture(mRenderer,
			("../res/maps/" + map->getImageName(0)).c_str());
	if (tex == nullptr)
		std::cout << "tex load failed" << std::endl;

	// test values
	int map_w = 1000;
	int map_h = 1000;

	for (int i = 0; i < map_w; i += 40)
		for (int j = 0; j < map_h; j += 40) {
			SDL_Rect dst;
			dst.x = i + xoffset;
			dst.y = j + yoffset;
			dst.w = 40;
			dst.h = 40;
			SDL_RenderCopy(mRenderer, tex, NULL, &dst);
		}

	/*

	 int w, h;

	 SDL_QueryTexture(tex, NULL, NULL, &w, &h);

	 SDL_Rect dst;
	 dst.x = 0;
	 dst.y = 0;
	 dst.w = w;
	 dst.h = h;

	SDL_RenderCopy(mRenderer, tex, NULL, &dst);

	*/
	//Render player
	SDL_Texture *tex2 = IMG_LoadTexture(mRenderer, "../player.png");
	SDL_Rect player_box = (player->getBoundingBox());

	SDL_RenderCopy(mRenderer, tex2, NULL, &player_box);

	SDL_RenderPresent(mRenderer);
}

///////////////////////////////////////////////////////////////////////////

void EngineCore::eventHandling(Input& input) {
	auto& actionList = input.getActions();
	SDL_Rect player_bbox = (player->getBoundingBox());
	for (auto& action : actionList) {
		if (action == InputAction::EXIT) {
			mainLoopQuit = true;
		}
		if (action == InputAction::MOVE_FORWARD) {
			xoffset -= 5;
			//player->moveRight();
		} else if (action == InputAction::MOVE_BACKWARD) {
			xoffset += 5;
			//player->moveLeft();
		}
		if (action == InputAction::JUMP) {
			player->jump();
		} else {
			player->fall();
		}

	}
}

