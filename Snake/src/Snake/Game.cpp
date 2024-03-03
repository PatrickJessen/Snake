#include "Game.h"
#include <fstream>
#include <iostream>
#include <string>

Game::Game(Map* map, int speed)
{
	this->map = map;
	this->speed = speed;
	dir = Direction::DOWN;
}

Game::~Game()
{
}

void Game::Update()
{
	if (!IsGameOver())
	{
		MoveSnake();
		SetDirection();
		EatApple();
	}
	else
	{
		if (Input::KeyPressed(Key::R))
		{
			map->ResetScore();
			map->Init();
			dir = Direction::DOWN;
		}

		if (Input::KeyPressed(Key::Y))
		{
			std::string score = std::to_string(map->GetScore()) + "\n";
			std::ofstream outFile;

			if (!outFile)
			{
				std::ofstream file("Assets/Scores.txt");

				file << score.c_str();
			}
			else
			{
				outFile.open("Assets/Scores.txt", std::ios_base::app);
				outFile << score.c_str();
			}


		}
	}
}

double Game::Movement(Direction action, State state)
{
	if (action == Direction::UP && dir != Direction::DOWN ||
		action == Direction::DOWN && dir != Direction::UP ||
		action == Direction::LEFT && dir != Direction::RIGHT ||
		action == Direction::RIGHT && dir != Direction::LEFT)
	{
		dir = action;
	}
	MoveSnake();
	// Define reward parameters
	double maxReward = 10.0; // Maximum reward for reaching food
	double minReward = -10.0; // Minimum reward for being close to walls
	double foodRewardScale = 1.0; // Scale factor for food reward
	double wallRewardScale = -0.1; // Scale factor for wall reward
	double bodyPenaltyScale = -1.0; // Scale factor for body penalty
	if (IsOutOfBounds()) {
		return -10.0;
	}
	else if (EatApple()) {
		return 10.0;
	}
	else if (HasBodyCollided()) {
		return -10.0;
	}

	// Calculate rewards based on distances
	//double foodReward = maxReward / (state.distanceToApple + 1); // Higher reward for closer distance to food
	return 0.0;
	//double wallReward = wallRewardScale * (1 / (state.distanceToWallUp + 1) + 1 / (state.distanceToWallDown + 1) +
	//	1 / (state.distanceToWallLeft + 1) + 1 / (state.distanceToWallRight + 1));
	//double bodyPenalty = bodyPenaltyScale * (1 / (state.distanceToBody + 1)); // Penalty for being close to body

	//// Ensure that the wall reward does not exceed the minimum reward
	//wallReward = std::min(wallReward, minReward);

	//// Total reward is a combination of food reward, wall reward, and body penalty
	//double totalReward = foodRewardScale * foodReward + wallRewardScale * wallReward + bodyPenaltyScale * bodyPenalty;

	//return totalReward;
}

void Game::MoveSnake()
{ 
	for (int i = map->GetSnake().size() - 1; i >= 0; i--)
	{
		if (i == 0)
		{
			if (dir == Direction::UP)
				map->GetSnake()[0]->Y -= speed;
			else if (dir == Direction::DOWN)
				map->GetSnake()[0]->Y += speed;
			else if (dir == Direction::LEFT)
				map->GetSnake()[0]->X -= speed;
			else if (dir == Direction::RIGHT)
				map->GetSnake()[0]->X += speed;
		}
		else
		{
			map->GetSnake()[i]->X = map->GetSnake()[i - 1]->X;
			map->GetSnake()[i]->Y = map->GetSnake()[i - 1]->Y;
		}
	}
}

State Game::GetCurrentState()
{
	State state;
	state.headX = map->GetSnake()[0]->X;
	state.headY = map->GetSnake()[0]->Y;
	state.direction = dir;
	calculateDistances(state, width, height);
	return state;
}

void Game::SetDirection()
{
	if (Input::KeyPressed(Key::W) && dir != Direction::DOWN)
		dir = Direction::UP;
	else if (Input::KeyPressed(Key::S) && dir != Direction::UP)
		dir = Direction::DOWN;
	else if (Input::KeyPressed(Key::A) && dir != Direction::RIGHT)
		dir = Direction::LEFT;
	else if (Input::KeyPressed(Key::D) && dir != Direction::LEFT)
		dir = Direction::RIGHT;
}

bool Game::EatApple()
{
	if (map->GetSnake()[0]->X == map->GetApple().X && map->GetSnake()[0]->Y == map->GetApple().Y)
	{
		map->AddSnake();
		RespawnApple();
		map->AddScore();
		return true;
	}
	return false;
}

void Game::RespawnApple()
{
	//srand(time(0));
	map->SetAppleXY(rand() % width, rand() % height);
}

bool Game::HasBodyCollided()
{
	for (int i = 1; i < map->GetSnake().size(); i++)
		if (map->GetSnake()[0]->X == map->GetSnake()[i]->X && map->GetSnake()[0]->Y == map->GetSnake()[i]->Y && map->GetSnake().size() > 2)
			return true;
	return false;
}

bool Game::IsOutOfBounds()
{
	if (map->GetSnake()[0]->X < 0 || map->GetSnake()[0]->X >= width || map->GetSnake()[0]->Y < 0 || map->GetSnake()[0]->Y >= height)
		return true;
	return false;
}

void Game::Reset()
{
	map->ResetScore();
	map->Init();
	dir = Direction::DOWN;
}

bool Game::IsGameOver()
{
	if (HasBodyCollided() || IsOutOfBounds())
		return true;
	return false;
}

double Game::Reward()
{
	if (EatApple()) {
		return 1.0;
	}
	else if (HasBodyCollided()) {
		return 0.0;
	}
	else if (IsOutOfBounds()) {
		return 0.0;
	}
	Snake* snake = map->GetSnake()[0];
	Apple apple = map->GetApple();
	double reward = 0.1;
	if (snake->X + 1 == apple.X || snake->Y + 1 == apple.X || snake->X - 1 == apple.X || snake->Y - 1 == apple.Y) {
		return 0.5;
	}
	return reward;
}

void Game::DistanceToWall(State& state)
{
	// Up
	state.distanceToWallUp = state.headY;

	// Down
	state.distanceToWallDown = height - 1 - state.headY;

	// Left
	state.distanceToWallLeft = state.headX;

	// Right
	state.distanceToWallRight = width - 1 - state.headX;
}

void Game::DistanceToApple(State& state)
{
	int dx = abs(state.headX - map->GetApple().X);
	int dy = abs(state.headY - map->GetApple().Y);
	state.distanceToApple = dx + dy;
}

bool Game::IsNearBody()
{
	return false;
}

double Game::calculateDistance(double x1, double y1, double x2, double y2)
{
	return std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
}

void Game::calculateDistances(State& state, double width, double height)
{
	// Distance to walls
	state.distanceToWallUp = state.headY;
	state.distanceToWallDown = height - state.headY;
	state.distanceToWallLeft = state.headX;
	state.distanceToWallRight = width - state.headX;

	Snake* snake = map->GetSnake()[0];
	if (snake->X < map->GetApple().X) {
		state.foodDirection = Direction::RIGHT;
	}
	else if (snake->X > map->GetApple().X) {
		state.foodDirection = Direction::LEFT;
	}
	else if (snake->Y < map->GetApple().Y) {
		state.foodDirection = Direction::UP;
	}
	else if (snake->Y > map->GetApple().Y) {
		state.foodDirection = Direction::DOWN;
	}
	// Distance to food
	//state.distanceToApple = calculateDistance(state.headX, state.headY, map->GetApple().X, map->GetApple().Y);
	double minDistance = std::numeric_limits<double>::max();

	for (const auto& segment : map->GetSnake()) {
		double segmentX = segment->X;
		double segmentY = segment->Y;
		double distance = calculateDistance(state.headX, state.headY, segmentX, segmentY);
		if (distance < minDistance) {
			minDistance = distance;
		}
	}
	state.distanceToBody = minDistance;
}
