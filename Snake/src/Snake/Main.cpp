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

double discountFactor = 1.0;
double epsilon = 0.9;
double learningFactor = 0.001;

void RunWithGui()
{
	Window* window = new Window("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 750, false);
	Draw* draw = new Draw(window);
	Game* game = new Game(1);
	QLearningAgent<Direction>* agent = new QLearningAgent<Direction>(width, learningFactor, discountFactor, epsilon);

	const int FPS = 10;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;
	bool show = true;
	bool slowDown = true;
	while (true)
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
}
void RunWithoutGui()
{
	Game* game = new Game(1);
	QLearningAgent<Direction>* agent = new QLearningAgent<Direction>(width, learningFactor, discountFactor, epsilon);
	while (true)
	{
		game->Reset();
		State state{};
		generation++;
		bool outOfBounds = false;
		int iterations = 0;
		while (!outOfBounds)
		{
			State currentState = game->GetCurrentState();
			Direction action = agent->chooseAction(currentState);

			double reward = game->Movement(action, currentState, iterations);
			State newState = game->GetCurrentState();
			if (reward == -20.0 || reward == -5.0) {
				outOfBounds = true;
			}
			agent->updateQValue(state, action, reward, newState);
			state = newState;
			if (generation == 20000) {
				agent->SaveTraining("test.txt");
				agent->SaveStateIndexMap("test2.txt");
				std::cout << "Saved\n";
			}
			if (game->GetMap()->GetHighScore() < game->GetMap()->GetScore()) {
				game->GetMap()->SetHighScore();
				std::cout << "Highscore reached: " << game->GetMap()->GetHighScore() << " generation: " << generation << "\n";
				if (game->GetMap()->GetHighScore() > 40) {
					agent->SaveTraining("test.txt");
					agent->SaveStateIndexMap("test2.txt");
					std::cout << "Saved\n";
				}
			}
			iterations++;
		}
	}
}
int main()
{
	bool useGui = false;
	if (useGui) {
		RunWithGui();
		return 0;
	}
	RunWithoutGui();

	return 0;
}