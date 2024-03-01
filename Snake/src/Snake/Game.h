#pragma once
#include "Input.h"
#include "Map.h"

enum class Direction
{
	UP, DOWN, LEFT, RIGHT
};

class Game
{
public:
	Game(Map* map, int speed);
	~Game();

public:
	void Update();
	double Movement(Direction action);
	bool HasBodyCollided();
	bool IsOutOfBounds();
	void Reset();
	Direction GetDirection() { return dir; }
	void MoveSnake();

private:
	void SetDirection();
	bool EatApple();
	void RespawnApple();
	bool IsGameOver();
	double Reward();

private:
	Direction dir;
	Map* map;
	int speed;
};

