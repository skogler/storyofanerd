#include "Input.hpp"
#include <SDL2/SDL_keycode.h>

Input::Input()
{
    mActionMappings[SDLK_ESCAPE] = InputAction::EXIT;

    mActionStates[SDLK_w] = InputAction::JUMP;
    mActionStates[SDLK_a] = InputAction::MOVE_BACKWARD;
    mActionStates[SDLK_d] = InputAction::MOVE_FORWARD;
}

Input::~Input()
{
}

