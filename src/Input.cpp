#include "Input.hpp"
#include <SDL2/SDL_keycode.h>

Input::Input()
{
    mActionMapping[SDLK_a] = InputAction::MOVE_BACKWARD;
    mActionMapping[SDLK_d] = InputAction::MOVE_FORWARD;
    mActionMapping[SDLK_w] = InputAction::JUMP;
    mActionMapping[SDLK_ESCAPE] = InputAction::EXIT;
}

Input::~Input()
{
}

