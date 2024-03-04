#pragma once
#include "Input.h"
#include "Map.h"

enum class Direction
{
	UP, DOWN, LEFT, RIGHT
};

struct State {
	int headX;
	int headY;
	Direction direction;
	Direction foodDirection;
	Direction nearestWall;
	bool wallUp;
	bool wallDown;
	bool wallLeft;
	bool wallRight;
	bool bodyInfront;
};

class Game
{
public:
	Game(Map* map, int speed);
	~Game();

public:
	void Update();
	double Movement(Direction action, State state, int& iterations);
	bool HasBodyCollided();
	bool IsOutOfBounds();
	void Reset();
	Direction GetDirection() { return dir; }
	void MoveSnake();
	State GetCurrentState();

private:
	void SetDirection();
	bool EatApple();
	void RespawnApple();
	bool IsGameOver();
	double Reward();
	void DistanceToWall(State& state);
	void DistanceToApple(State& state);
	bool IsNearBody();
	double calculateDistance(double x1, double y1, double x2, double y2);
	void calculateDistances(State& state, double width, double height);

private:
	Direction dir;
	Map* map;
	int speed;
};

