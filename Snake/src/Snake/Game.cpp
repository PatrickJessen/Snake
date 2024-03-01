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

double Game::Movement(Direction action)
{
	dir = action;
	/*if (EatApple()) {
		return 1.0;
	}
	else if (HasBodyCollided()) {
		return 0.0;
	}
	else if (IsOutOfBounds()) {
		return -1.0;
	}
	return 0.1;*/
	return Reward();
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
	/*for (int i = 0; i < 3; i++) {
		if (map->field[snake[0]->X - i][snake[0]->Y] == map->field[map->GetApple().X][map->GetApple().Y] ||
			map->field[snake[0]->X][snake[0]->Y - i] == map->field[map->GetApple().X][map->GetApple().Y] ||
			map->field[snake[0]->X][snake[0]->Y - i] == map->field[map->GetApple().X][map->GetApple().Y] ||
			map->field[snake[0]->X - i][snake[0]->Y] == map->field[map->GetApple().X][map->GetApple().Y] ||
			map->field[snake[0]->X + i][snake[0]->Y] == map->field[map->GetApple().X][map->GetApple().Y] ||
			map->field[snake[0]->X][snake[0]->Y + i] == map->field[map->GetApple().X][map->GetApple().Y] ||
			map->field[snake[0]->X][snake[0]->Y + 1] == map->field[map->GetApple().X][map->GetApple().Y] ||
			map->field[snake[0]->X + 1][snake[0]->Y] == map->field[map->GetApple().X][map->GetApple().Y]) {
			reward += 0.1;
		}
	}*/
	/*if (map->field[snake[0]->X - 1][snake[0]->Y] == map->field[map->GetApple().X][map->GetApple().Y] ||
		map->field[snake[0]->X][snake[0]->Y - 1] == map->field[map->GetApple().X][map->GetApple().Y]) {
		return 0.5;
	}
	if (map->field[snake[0]->X][snake[0]->Y -1] == map->field[map->GetApple().X][map->GetApple().Y] ||
		map->field[snake[0]->X -1][snake[0]->Y] == map->field[map->GetApple().X][map->GetApple().Y]) {
		return 0.5;
	}
	if (map->field[snake[0]->X +1][snake[0]->Y] == map->field[map->GetApple().X][map->GetApple().Y] ||
		map->field[snake[0]->X][snake[0]->Y +1] == map->field[map->GetApple().X][map->GetApple().Y]) {
		return 0.5;
	}
	if (map->field[snake[0]->X][snake[0]->Y + 1] == map->field[map->GetApple().X][map->GetApple().Y] ||
		map->field[snake[0]->X + 1][snake[0]->Y] == map->field[map->GetApple().X][map->GetApple().Y]) {
		return 0.5;
	}*/
	return reward;
}
