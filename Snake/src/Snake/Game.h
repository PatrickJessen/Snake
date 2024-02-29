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

private:
	void MoveSnake();
	void SetDirection();
	bool EatApple();
	void RespawnApple();
	bool IsGameOver();


private:
	Direction dir;
	Map* map;
	int speed;
};

