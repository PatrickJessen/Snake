#include "Window.h"
#include "Draw.h"
#include "Game.h"
#include "QLearningAgent.h"
#include <thread>



int main()
{
	Window* window = new Window("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 1000, false);
	Draw* draw = new Draw(window);
	Game* game = new Game(draw->GetMap(), 1);
	QLearningAgent<Direction>* agent = new QLearningAgent<Direction>(width, 0.1, 0.9, 1.5);
	const int FPS = 30;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;
	while (window->Running())
	{
		game->Reset();
		int state = 0;
		while (!game->IsOutOfBounds())
		{
			frameStart = SDL_GetTicks();
			frameTime = SDL_GetTicks() - frameStart;

			if (frameDelay > frameTime)
			{
				SDL_Delay(frameDelay - frameTime);
			}
			Direction action = agent->chooseAction(state);
			double reward = game->Movement(action);
			int newState = draw->GetMap()->GetSnake()[0]->X * width + draw->GetMap()->GetSnake()[0]->Y;
			agent->updateQValue(state, action, reward, newState);
			state = newState;
			draw->Update();
			window->Update();
			window->Clear();
		}
	}

	window->Clean();

	return 0;
}