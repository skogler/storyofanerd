#ifndef INPUT_HPP
#define INPUT_HPP

#include <map>
#include <vector>
#include <type_traits>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

enum class InputAction
{
    MOVE_FORWARD,
    MOVE_BACKWARD,
    JUMP,
    EXIT,
    NONE
};

class Input
{
public:
    Input ();
    virtual ~Input ();

    inline const std::vector<InputAction>& getActions();

private:
    Input (const Input& other);
    Input& operator= (const Input& other);

protected:
    /* data */
    SDL_Event mPolledEvent;
    std::vector<InputAction> mActions;
    std::map<SDL_Keycode, InputAction> mActionMappings;
    std::map<SDL_Keycode, InputAction> mActionStates;
};


const std::vector<InputAction>& Input::getActions()
{
    mActions.clear();
    while (SDL_PollEvent(&mPolledEvent)) {
        if (mPolledEvent.type == SDL_QUIT) {
            mActions.push_back(InputAction::EXIT);
            return mActions;
        }
        if (mPolledEvent.type == SDL_KEYDOWN || mPolledEvent.type == SDL_KEYUP) {
            const auto& actionMappingIterator = mActionMappings.find(mPolledEvent.key.keysym.sym);
            if (actionMappingIterator != mActionMappings.end()) {
                const auto& actionStateIterator = mActionStates.find(mPolledEvent.key.keysym.sym);
                if (actionStateIterator == mActionStates.end()) {
                    // If not state cached just add the action
                    if (mPolledEvent.type == SDL_KEYDOWN) {
                        mActions.push_back(actionMappingIterator->second);
                    }
                }
            }
        }
    }
    SDL_PumpEvents();
    const Uint8* state = SDL_GetKeyboardState(NULL);
    for (const auto& actionState : mActionStates) {
        if (state[SDL_GetScancodeFromKey(actionState.first)])
            mActions.push_back(actionState.second);
    }
    return mActions;
}

#endif /* end of include guard: INPUT_HPP */
