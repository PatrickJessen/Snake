#include "Window.h"
#include "Draw.h"
#include "Game.h"
#include "QLearningAgent.h"
#include "NeuralNetwork.h"
#include <thread>

int generation = 0;
int highestScore = 0;
int CurrentHighScore(int score)
{
	if (score > highestScore) {
		highestScore = score;
	}
	return highestScore;
}

int main()
{
	Window* window = new Window("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 750, false);
	Game* game = new Game(1);
	Draw* draw = new Draw(window);
	QLearningAgent<Direction>* agent = new QLearningAgent<Direction>(width, 0.1, 0.99, 0.001);

	const int FPS = 10;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;
	bool show = true;
	bool slowDown = true;
	while (window->Running())
	{
		game->Reset();
		State state{};
		generation++;
		bool outOfBounds = false;
		int iterations = 0;
		while (!outOfBounds)
		{
			if (Input::KeyPressed(Key::S)) {
				agent->SaveTraining("test.txt");
				agent->SaveStateIndexMap("test2.txt");
				std::cout << "Saved\n";
			}
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
			State currentState = game->GetCurrentState();
			Direction action = agent->chooseAction(currentState);

			double reward = game->Movement(action, currentState, iterations);
			State newState = game->GetCurrentState();
			if (reward == -20.0 || reward == -5.0) {
				outOfBounds = true;
			}
			agent->updateQValue(state, action, reward, newState);
			state = newState;
			if (game->GetMap()->GetHighScore() < game->GetMap()->GetScore()) {
				game->GetMap()->SetHighScore();
				std::cout << "Highscore reached: " << game->GetMap()->GetHighScore() << " generation: " << generation << "\n";
			}
			if (show) {
				draw->SetHighScore(game->GetMap()->GetHighScore());
				draw->Update(game->GetMap());
			}
			window->Update();
			window->Clear();
			iterations++;
		}
	}

	window->Clean();

	return 0;
}