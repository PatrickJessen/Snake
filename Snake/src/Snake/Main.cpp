#include "Window.h"
#include "Draw.h"
#include "Game.h"
#include "QLearningAgent.h"
#include <thread>



int main()
{
	Window* window = new Window("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 750, false);
	Draw* draw = new Draw(window);
	Game* game = new Game(draw->GetMap(), 1);
	QLearningAgent<Direction>* agent = new QLearningAgent<Direction>(width, 1.0, 1.0, 1.0);
	const int FPS = 10;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;
	int generation = 0;
	bool show = true;
	bool slowDown = false;
	while (window->Running())
	{
		game->Reset();
		int state = 0;
		generation++;
		while (!game->IsOutOfBounds())
		{
			if (Input::KeyPressed(Key::L)) {
				show = !show;
				std::cout << "Generation: " << generation << "\n";
			}
			if (Input::KeyPressed(Key::K)) {
				slowDown = !slowDown;
			}
			if (slowDown) {

				frameStart = SDL_GetTicks();
				frameTime = SDL_GetTicks() - frameStart;

				if (frameDelay > frameTime)
				{
					SDL_Delay(frameDelay - frameTime);
				}
			}
			Direction action = agent->chooseAction(state);
			if (action == Direction::UP && game->GetDirection() != Direction::DOWN ||
				action == Direction::DOWN && game->GetDirection() != Direction::UP ||
				action == Direction::LEFT && game->GetDirection() != Direction::RIGHT ||
				action == Direction::RIGHT && game->GetDirection() != Direction::LEFT)
			{

				double reward = game->Movement(action);
				int newState = draw->GetMap()->GetSnake()[0]->X * width + draw->GetMap()->GetSnake()[0]->Y;
				agent->updateQValue(state, action, reward, newState);
				state = newState;
			}
			if (draw->GetHighestScore() < draw->GetMap()->GetScore()) {
				draw->SetHighScore(draw->GetMap()->GetScore());
				std::cout << "Highscore reached: " << draw->GetHighestScore() << " generation: " << generation << "\n";
			}
			if (show) {
				draw->Update();
			}
			game->MoveSnake();
			window->Update();
			window->Clear();
		}
	}

	window->Clean();

	return 0;
}