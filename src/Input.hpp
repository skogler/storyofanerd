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
    std::map<InputAction, bool> mActionStates;
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
                const auto& actionStateIterator = mActionStates.find(actionMappingIterator->second);
                if (actionStateIterator != mActionStates.end()) {
                    if (mPolledEvent.type == SDL_KEYUP) {
                        actionStateIterator->second = false;
                    } else {
                        actionStateIterator->second = true;
                    }
                } else {
                    // If not state cached just add the action
                    if (mPolledEvent.type == SDL_KEYDOWN) {
                        mActions.push_back(actionMappingIterator->second);
                    }
                }
            }
        }
    }
    for (const auto& actionState : mActionStates) {
        if (actionState.second) {
            mActions.push_back(actionState.first);
        }
    }
    return mActions;
}

#endif /* end of include guard: INPUT_HPP */
