#include "Window.h"
#include "Draw.h"
#include "Game.h"
#include <thread>



int main()
{
	Window* window = new Window("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 750, false);
	Draw* draw = new Draw(window);
	Game* game = new Game(draw->GetMap(), 1);

	const int FPS = 30;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;
	while (window->Running())
	{
		frameStart = SDL_GetTicks();
		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}

		game->Update();
		draw->Update();
		window->Update();
		window->Clear();
	}

	window->Clean();

	return 0;
}