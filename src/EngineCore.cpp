#include "EngineCore.hpp"
#include <cstdlib>
#include <sstream>

EngineCore::EngineCore() :
		mMainLoopQuit(false), mWindow(nullptr), mRenderer(nullptr), mViewport( {
				0, 0, 640, 480 }), mPlayer(nullptr), mAudio(nullptr), mPlayerImage(
				nullptr) {
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
	SDL_SetRenderDrawColor(mRenderer, 0xF0, 0xF0, 0xF0, 0xF0);

	// Initialize Audio after SDL
	mAudio.reset(new Audio());
	mPlayer.reset(new Player());
	mPlayerImage = IMG_LoadTexture(mRenderer, "../player.png");

	//load the map | TODO: error handling
	map = new LoadedMap("../res/maps/testmap.tmx");
	map->loadFile();

	tileSet = IMG_LoadTexture(mRenderer,
			("../res/maps/" + map->getImageName(0)).c_str());
	if (tileSet == nullptr)
		std::cout << "tilesetLoadfailed" << std::endl;
	generateTilesetResources();
}

EngineCore::~EngineCore() {
	delete map;
	if (tileSet) {
		SDL_DestroyTexture(tileSet);
	}
	if (mPlayerImage) {
		SDL_DestroyTexture(mPlayerImage);
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
	mAudio->setSoundEffectVolume(128);
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
	mPlayer->update(delta);
	mViewport.x = mPlayer->getBoundingBox().x - mViewport.w / 2;
}
///////////////////////////////////////////////////////////////////////////

void EngineCore::render() {
	//Clear screen
	SDL_RenderClear(mRenderer);

	//vector<Tile> tile_vec = map->getTileSetVector(0);

	//Parse tile data
	string tile_data = map->getLayerData(0);

	std::vector<int> tile_data_ints;
	std::stringstream ss(tile_data);
	int vec_index;
	while (ss >> vec_index) {
		tile_data_ints.push_back(vec_index);
		if (ss.peek() == ',')
			ss.ignore();
	}

	int margin = map->getTileSetMargin(0);
	int spacing = map->getTileSetSpacing(0);
	int x_coord = 0;
	int y_coord = 480
			- (map->getTileMap().height * map->getTileMap().tileheight);
	int tile_width = map->getTileMap().tilewidth  ;
	int tiles_in_row = map->getTileMap().width;

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
			y_coord += map->getTileMap().tileheight ;
			x_coord = 0;
		}
		space_index++;
	}

	//Render mPlayer
	SDL_Rect dst(mPlayer->getBoundingBox());
	dst.x -= mViewport.x;
	dst.y -= (mViewport.y - dst.h);
	SDL_RenderCopy(mRenderer, mPlayerImage, NULL, &dst);
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
			int index = random() % 3 + 1;
			std::stringstream filename;
			filename << "jump" << index << ".wav";
			mAudio->playSound(filename.str());
			mPlayer->jump();
		}
	}
}

void EngineCore::generateTilesetResources() {

	int tile_w = map->getTileMap().tilewidth;
	int tile_h = map->getTileMap().tileheight;
	std::cout << tile_w << " -  " << tile_h << std::endl;

	int margin = map->getTileSetMargin(0);
	int spacing = map->getTileSetSpacing(0);


	for (int j = 0; j <= 8; j++)
	for (int i = 0; i <=6; i++) {

		SDL_Rect rect;
		rect.x = j + tile_w;
		rect.y = i + tile_h;
		rect.w = tile_w;
		rect.h = tile_h;
		tileClips.push_back(rect);
	}

	current_clip = 0;
}

