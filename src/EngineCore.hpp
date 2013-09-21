#ifndef ENGINECORE_HPP
#define ENGINECORE_HPP

struct SDL_Window;
struct SDL_Renderer;


class EngineCore
{
public:
	EngineCore();
	virtual ~EngineCore();

    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;

    void executeLoop();
    void render();
    void eventHandling();


private:
	EngineCore(const EngineCore& other);
	EngineCore& operator=(const EngineCore& other);



};

#endif /* end of include guard: ENGINECORE_HPP */
