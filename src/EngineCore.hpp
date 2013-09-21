
struct SDL_Window;
struct SDL_Renderer;


class EngineCore
{
public:
	EngineCore();
	virtual ~EngineCore();

    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;

private:
	EngineCore(const EngineCore& other);
	EngineCore& operator=(const EngineCore& other);

};
