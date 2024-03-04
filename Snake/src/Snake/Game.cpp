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

double Game::Movement(Direction action, State state, int& iterations)
{
	if (action == Direction::UP && dir != Direction::DOWN ||
		action == Direction::DOWN && dir != Direction::UP ||
		action == Direction::LEFT && dir != Direction::RIGHT ||
		action == Direction::RIGHT && dir != Direction::LEFT)
	{
		dir = action;
	}
	MoveSnake();

	if (IsOutOfBounds()) {
		return -10.0;
	}
	else if (EatApple()) {
		iterations = 0;
		return 10.0;
	}
	else if (HasBodyCollided()) {
		return -10.0;
	}
	if (iterations > 50) {
		return -1.0;
	}
	return 0.0;
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
	calculateDistances(state, width, height);
	DistanceToApple(state);
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
	for (int i = 0; i < map->GetSnake().size(); i++) {
		if (map->GetApple().X == map->GetSnake()[i]->X && map->GetApple().Y == map->GetSnake()[i]->Y) {
			RespawnApple();
		}
	}
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
	if (map->GetSnake()[0]->X < 0 || map->GetSnake()[0]->X >= width || map->GetSnake()[0]->Y < 0 || map->GetSnake()[0]->Y >= height) {
		//std::cout << map->GetSnake()[0]->X << ", " << map->GetSnake()[0]->Y << "\n";
		return true;
	}
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
}

void Game::DistanceToApple(State& state)
{
	int dx = abs(state.headX - map->GetApple().X);
	int dy = abs(state.headY - map->GetApple().Y);
	//state.distanceToApple = dx + dy;
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
	Snake* snake = map->GetSnake()[0];
	state.headX = snake->X;
	state.headY = snake->Y;
	state.direction = dir;
	state.wallDown = false;
	state.wallLeft = false;
	state.wallRight = false;
	state.wallUp = false;
	state.bodyInfront = false;
	if (snake->Y -1 <= 0) {
		state.nearestWall = Direction::UP;
		state.wallUp = true;
	}
	else if (snake->Y +1 >= height) {
		state.nearestWall = Direction::DOWN;
		state.wallDown = true;
	}
	if (snake->X -1 <= 0) {
		state.nearestWall = Direction::LEFT;
		state.wallLeft = true;
	}
	else if (snake->X +1 >= width) {
		state.nearestWall = Direction::RIGHT;
		state.wallRight = true;
	}

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

	if (map->GetSnake().size() > 2) {
		for (int i = 1; i < map->GetSnake().size(); i++) {
			if (state.direction == Direction::DOWN || state.direction == Direction::UP) {
				if (snake->X - 1 == map->GetSnake()[i]->X && snake->Y == map->GetSnake()[i]->Y ||
					snake->X + 1 == map->GetSnake()[i]->X && snake->Y == map->GetSnake()[i]->Y) {
					state.bodyInfront = true;
				}
			}
			else if (state.direction == Direction::LEFT || state.direction == Direction::RIGHT) {
				if (snake->Y - 1 == map->GetSnake()[i]->Y && snake->X == map->GetSnake()[i]->X ||
					snake->Y + 1 == map->GetSnake()[i]->Y && snake->X == map->GetSnake()[i]->X) {
					state.bodyInfront = true;
				}
			}
		}
	}
}
