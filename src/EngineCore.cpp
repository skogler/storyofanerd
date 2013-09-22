#include "EngineCore.hpp"
#include <cstdlib>
#include <sstream>

EngineCore::EngineCore() :
		mMainLoopQuit(false), mWindow(nullptr), mRenderer(nullptr), mViewport( {
				0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }), mPlayer(nullptr), mAudio(
				nullptr), backgroundRect( {
	0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		throw std::runtime_error("Could not initialize the SDL2 library!");
	}

	mWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED,
	SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
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
	SDL_SetRenderDrawColor(mRenderer, 0xF0, 0xF0, 0xF0, 0xF0);

	// Initialize Audio after SDL
	mAudio.reset(new Audio());
	//mPlayerImage = IMG_LoadTexture(mRenderer, "../player.png");

	//load the mMap | TODO: error handling
	mMap.reset(new LoadedMap("../res/maps/testmap.tmx"));
	mMap->loadFile();

	SDL_Surface* tileSetSurface  = IMG_Load(("../res/maps/" + mMap->getImageName(0)).c_str());
	tileSet = SDL_CreateTextureFromSurface(mRenderer, tileSetSurface);
	if (tileSet == nullptr)
		std::cout << "tilesetLoadfailed" << std::endl;

	background = IMG_LoadTexture(mRenderer, ("../res/images/background.png"));

	generateTilesetResources(tileSetSurface->w, tileSetSurface->h);
    mMap->calculateCollisionGeometry();
    // Load player last!
	mPlayer.reset(new Player(mMap, mAudio, mViewport));
	mPlayer->loadAnimations(mRenderer);
}

EngineCore::~EngineCore() {
	if (tileSet) {
		SDL_DestroyTexture(tileSet);
	}
	if (mRenderer) {
		SDL_DestroyRenderer(mRenderer);
	}
	if (mWindow) {
		SDL_DestroyWindow(mWindow);
	}
	SDL_Quit();
}

///////////////////////////////////////////////////////////////////////////

void EngineCore::start() {
	Input input;
	mAudio->play();
	mAudio->setSoundEffectVolume(80);
	mAudio->setMusicVolume(64);

	//FPS variables
	int sdl_last_tick = 0;
	int sdl_new_tick = 0;
	int delta = 0;
	int sdl_fps_intervall = 1000 / FPS;

	while (!mMainLoopQuit) {
		eventHandling(input);
		update(delta);
		render();

		//FPS code
		sdl_new_tick = SDL_GetTicks();

		delta = sdl_new_tick - sdl_last_tick;
		if (delta < sdl_fps_intervall) {
			SDL_Delay(sdl_fps_intervall - delta);
		}
		sdl_last_tick = sdl_new_tick;

	}
}
///////////////////////////////////////////////////////////////////////////
void EngineCore::update(int delta) {
    SDL_GetWindowSize(mWindow, &mViewport.w, &mViewport.h);
	mPlayer->update(delta);
	mViewport.x = mPlayer->getBoundingBox().x - mViewport.w / 2;
    backgroundRect.w = mViewport.w;
    backgroundRect.h = mViewport.h;
}
////////////////////////////////////////////////////////////////////////	///

void EngineCore::render() {
    //SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	//Clear screen
	SDL_RenderClear(mRenderer);

	SDL_RenderCopy(mRenderer, background, NULL, &backgroundRect);

	//Parse tile data
	string tile_data = mMap->getLayerData(0);

	std::vector<int> tile_data_ints;
	std::stringstream ss(tile_data);
	int vec_index;
	while (ss >> vec_index) {
		tile_data_ints.push_back(vec_index);
		if (ss.peek() == ',')
			ss.ignore();
	}

	int margin = mMap->getTileSetMargin(0);
	int spacing = mMap->getTileSetSpacing(0);
	int x_coord = 0;
	int y_coord = mViewport.h
			- (mMap->getTileMap().height * mMap->getTileMap().tileheight);
	int tile_width = mMap->getTileMap().tilewidth;
	int tiles_in_row = mMap->getTileMap().width;

	int space_index = 0;
	for (std::vector<int>::iterator it = tile_data_ints.begin();
			it != tile_data_ints.end(); ++it) {
		current_clip = *it;
		SDL_Rect dst;
		dst.x = x_coord - mViewport.x;
		dst.y = y_coord - mViewport.y;
		dst.w = tileClips.at(current_clip).w;
		dst.h = tileClips.at(current_clip).h;

		if (current_clip != 0)
			SDL_RenderCopy(mRenderer, tileSet, &(tileClips.at(current_clip)),
					&dst);

		x_coord += tile_width;
		if (x_coord == (tiles_in_row * tile_width)) {
			y_coord += mMap->getTileMap().tileheight;
			x_coord = 0;
		}
		space_index++;
	}

	//Render mPlayer
	SDL_Rect dst(mPlayer->getBoundingBox());
	dst.x -= mViewport.x;
	dst.y -= mViewport.y;
	SDL_RenderCopy(mRenderer, mPlayer->getPlayerImage(), NULL, &dst);

	SDL_RenderPresent(mRenderer);
}

///////////////////////////////////////////////////////////////////////////

void EngineCore::eventHandling(Input& input) {
	auto& actionList = input.getActions();
	SDL_Rect mPlayer_bbox = (mPlayer->getBoundingBox());
	for (auto& action : actionList) {
		if (action == InputAction::EXIT) {
			mMainLoopQuit = true;
		}
		if (action == InputAction::MOVE_FORWARD) {
			mPlayer->moveRight();
		} else if (action == InputAction::MOVE_BACKWARD) {
			mPlayer->moveLeft();
		}
		if (action == InputAction::JUMP) {
			mPlayer->jump();
		}
	}
}

void EngineCore::generateTilesetResources(int tileSetWidth, int tileSetHeight) {
	int tile_w = mMap->getTileMap().tilewidth;
	int tile_h = mMap->getTileMap().tileheight;

	int margin = mMap->getTileSetMargin(0);
	int spacing = mMap->getTileSetSpacing(0);

	int size = mMap->getTileSetVector(0).size();

	int tileWcheck = tileSetWidth / tile_w;
	int tileHcheck = tileSetHeight / tile_h;
	for (int j = 0; j <= tileWcheck; j++)
		for (int i = 0; i <= tileHcheck; i++) {
			//TODO: add margin for the black lines top and left
			SDL_Rect rect;
			rect.x = j + tile_w;
			rect.y = i + tile_h;
			rect.w = tile_w;
			rect.h = tile_h;
			tileClips.push_back(rect);
		}

	current_clip = 0;
}

