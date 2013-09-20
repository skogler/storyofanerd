#include "SDL.h"

#include "common.h"
#include "errorcodes.h"

int main()
{
	SDL_Init(SDL_INIT_HAPTIC);
	SDL_Quit();
	return 0;
}
