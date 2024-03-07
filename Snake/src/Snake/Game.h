#pragma once
#include "Input.h"
#include "Map.h"

enum class Direction
{
	UP, DOWN, LEFT, RIGHT
};

enum class Danger
{
	NONE, LEFT, RIGHT, UP, DOWN, DOWNLEFT, DOWNRIGHT, UPRIGHT, UPLEFT
};

struct State {
	Direction direction;
	Direction foodDirection;
	Danger body;
	Danger wall;
	bool operator==(const State& other) const {
		return
			foodDirection == other.foodDirection &&
			body == other.body &&
			wall == other.wall &&
			direction == other.direction;
	}
};

class Game
{
public:
	Game(int speed);
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
	Map* GetMap() { return map; }
	bool GetOutOfBounds() { return outOfBounds; }
	bool GetBodyCollided() { return bodyCollided; }

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
	bool outOfBounds = false;
	bool bodyCollided = false;
};

