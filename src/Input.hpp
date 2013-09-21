#ifndef INPUT_HPP
#define INPUT_HPP

#include <map>

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

    /// Call this until InputAction::NONE is returned
    inline InputAction pollAction();

private:
    Input (const Input& other);
    Input& operator= (const Input& other);

protected:
    /* data */
    std::map<SDL_Keycode, InputAction> mActionMapping;
};

InputAction Input::pollAction()
{
    SDL_Event polledEvent;
    if (SDL_PollEvent(&polledEvent)) {
        if (polledEvent.type == SDL_QUIT) {
            return InputAction::EXIT;
        }
        if (polledEvent.type == SDL_KEYDOWN) {
            const auto& actionMappingIterator = mActionMapping.find(polledEvent.key.keysym.sym);
            if (actionMappingIterator != mActionMapping.end()) {
                return actionMappingIterator->second;
            }
        }
    }
    return InputAction::NONE;
}

#endif /* end of include guard: INPUT_HPP */
