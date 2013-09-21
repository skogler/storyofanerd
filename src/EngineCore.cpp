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

	tileSet = IMG_LoadTexture(mRenderer,
			("../res/maps/" + map->getImageName(0)).c_str());
	if(tileSet == nullptr)
		std::cout << "tilesetLoadfailed" << std::endl;
	generateTilesetResources();
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
	SDL_RenderClear(mRenderer);

	vector<Tile> tile_vec = map->getTileSetVector(0);

	//Parse tile data
    string tile_data = map->getLayerData(0);

    std::vector<int> tile_data_ints;
    std::stringstream ss(tile_data);
    int vec_index;
    while (ss >> vec_index)
    {
    	tile_data_ints.push_back(vec_index);
    	if (ss.peek() == ',')
    		ss.ignore();
    }

    int x_coord = 0;
    int y_coord = 0;
    int row_count = map->getTileMap().width / map->getTileMap().tilewidth;
    for(std::vector<int>::iterator it = tile_data_ints.begin(); it != tile_data_ints.end(); ++it) {
    		current_clip = *it;
    		x_coord += map->getTileMap().tilewidth;
    	    if(x_coord >= (row_count *  map->getTileMap().tilewidth))
    	    {
    	    	y_coord += map->getTileMap().tileheight;
    	    	x_coord = 0;
    	    }

			SDL_Rect dst;
			dst.x = x_coord + xoffset;
			dst.y = y_coord + yoffset;
			dst.w = tileClips.at(current_clip).w;
			dst.h = tileClips.at(current_clip).h;

			SDL_RenderCopy(mRenderer, tileSet, &tileClips[current_clip], &dst);
    	}
    /*
	for (int i = 0; i < map_w; i += 40)
		for (int j = 0; j < map_h; j += 40) {
			SDL_Rect dst;
			dst.x = i + xoffset;
			dst.y = j + yoffset;
			dst.w = tileClips.at(current_clip).w;
			dst.h = tileClips.at(current_clip).h;
			SDL_RenderCopy(mRenderer, tileSet, &tileClips[current_clip], &dst);

		}*/

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

void EngineCore::generateTilesetResources() {

	if (tileSet == nullptr)
		std::cout << "tileSet couldnt be populated" << std::endl;

	int tile_w = map->getTileMap().tilewidth;
	int tile_h = map->getTileMap().tileheight;;
	int x = SCREEN_WIDTH / 2 - tile_w / 2;
	int y = SCREEN_HEIGHT / 2 - tile_h / 2;

	for (int i = 0; i < 48; ++i) {
		SDL_Rect rect;
		rect.x = i / 2 * tile_w;
		rect.y = i % 2 * tile_h;
		rect.w = tile_w;
		rect.h = tile_h;
		tileClips.push_back(rect);
	}



	current_clip = 0;
}

