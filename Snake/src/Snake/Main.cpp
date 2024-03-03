#include "Window.h"
#include "Draw.h"
#include "Game.h"
#include "QLearningAgent.h"
#include "NeuralNetwork.h"
#include <thread>



int main()
{
	Window* window = new Window("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 750, false);
	Draw* draw = new Draw(window);
	Game* game = new Game(draw->GetMap(), 1);
	QLearningAgent<Direction>* agent = new QLearningAgent<Direction>(width, 0.1, 0.9, 0.1);
	//Agent agent2;
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
		State state{};
		generation++;
		bool outOfBounds = false;
		while (!outOfBounds)
		{
			if (Input::KeyPressed(Key::A)) {
				agent->setLearningRate(0.9);
			}
			if (Input::KeyPressed(Key::S)) {
				agent->setLearningRate(0.1);
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

			double reward = game->Movement(action, currentState);
			State newState = game->GetCurrentState();
			if (reward == -10.0) {
				outOfBounds = true;
			}
			agent->updateQValue(state, action, reward, newState);
			state = newState;
			if (draw->GetHighestScore() < draw->GetMap()->GetScore()) {
				draw->SetHighScore(draw->GetMap()->GetScore());
				std::cout << "Highscore reached: " << draw->GetHighestScore() << " generation: " << generation << "\n";
			}
			if (show) {
				draw->Update();
			}
			window->Update();
			window->Clear();
		}
	}

	window->Clean();

	return 0;
}