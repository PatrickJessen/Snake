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
	void Movement();

private:
	void MoveSnake();
	void SetDirection();
	void EatApple();
	void RespawnApple();
	bool HasBodyCollided();
	bool IsOutOfBounds();
	bool IsGameOver();


private:
	Direction dir;
	Map* map;
	int speed;
};

