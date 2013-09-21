#include "Input.hpp"
#include <SDL2/SDL_keycode.h>

Input::Input()
{
    mActionMappings[SDLK_a] = InputAction::MOVE_BACKWARD;
    mActionMappings[SDLK_d] = InputAction::MOVE_FORWARD;
    mActionMappings[SDLK_w] = InputAction::JUMP;
    mActionMappings[SDLK_ESCAPE] = InputAction::EXIT;

    mActionStates[InputAction::MOVE_FORWARD] = false;
    mActionStates[InputAction::MOVE_BACKWARD] = false;
}

Input::~Input()
{
}

