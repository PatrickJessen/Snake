#include "Game.h"
#include <fstream>
#include <iostream>
#include <string>

Game::Game(int speed)
{
	this->speed = speed;
	dir = Direction::DOWN;
	map = new Map();
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
		outOfBounds = true;
		return -20.0;
	}
	else if (EatApple()) {
		iterations = 0;
		return 10.0;
	}
	else if (HasBodyCollided()) {
		bodyCollided = true;
		return -20.0;
	}
	if (iterations > width * height) {
		return -5.0;
	}
	return -0.1;
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
	outOfBounds = false;
	bodyCollided = false;
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
	/*int dx = abs(state.headX - map->GetApple().X);
	int dy = abs(state.headY - map->GetApple().Y);*/
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
	state.direction = dir;
	state.wall = Danger::NONE;
	state.body = Danger::NONE;
	//state.size = 0;
	//if (map->GetSnake().size() >= height - 1) {
	//	state.size++;
	//}
	/*for (int i = height - 1; i < map->GetSnake().size(); i++) {
		state.size++;
	}*/

	if (snake->Y -1 < 0) {
		//state.nearestWall = Direction::UP;
		state.wall = Danger::UP;
	}
	else if (snake->Y +1 > height -1) {
		//state.nearestWall = Direction::DOWN;
		state.wall = Danger::DOWN;
	}
	if (snake->X -1 < 0) {
		//state.nearestWall = Direction::LEFT;
		state.wall = Danger::LEFT;
	}
	else if (snake->X +1 > width -1) {
		//state.nearestWall = Direction::RIGHT;
		state.wall = Danger::RIGHT;
	}

	if (snake->Y - 1 < 0 && snake->X - 1 < 0) {
		state.wall = Danger::UPLEFT;
	}
	else if (snake->Y - 1 < 0 && snake->X + 1 > width - 1) {
		state.wall = Danger::UPRIGHT;
	}
	else if (snake->Y + 1 > height -1 && snake->X + 1 > width - 1) {
		state.wall = Danger::DOWNRIGHT;
	}
	else if (snake->Y + 1 < height -1 && snake->X - 1 < 0) {
		state.wall = Danger::DOWNLEFT;
	}

	if (snake->X < map->GetApple().X) {
		state.foodDirection = Direction::RIGHT;
	}
	else if (snake->X > map->GetApple().X) {
		state.foodDirection = Direction::LEFT;
	}
	else if (snake->Y < map->GetApple().Y) {
		state.foodDirection = Direction::DOWN;
	}
	else if (snake->Y > map->GetApple().Y) {
		state.foodDirection = Direction::UP;
	}

	if (map->GetSnake().size() > 2) {
		for (int i = 1; i < map->GetSnake().size(); i++) {
			Snake* tail = map->GetSnake()[i];
			if (snake->X - 1 == tail->X && snake->Y == tail->Y) {
				state.body = Danger::LEFT;
			}
			else if (snake->X + 1 == tail->X && snake->Y == tail->Y) {
				state.body = Danger::RIGHT;
			}
			else if (snake->X == tail->X && snake->Y -1 == tail->Y) {
				state.body = Danger::UP;
			}
			else if (snake->X == tail->X && snake->Y + 1 == tail->Y) {
				state.body = Danger::DOWN;
			}
			/*if (snake->X > tail->X && snake->Y == tail->Y) {
				state.body = Danger::LEFT;
			}
			else if (snake->X < tail->X && snake->Y == tail->Y) {
				state.body = Danger::RIGHT;
			}
			else if (snake->X == tail->X && snake->Y < tail->Y) {
				state.body = Danger::DOWN;
			}
			else if (snake->X == tail->X && snake->Y > tail->Y) {
				state.body = Danger::UP;
			}*/
		}
	}
}
