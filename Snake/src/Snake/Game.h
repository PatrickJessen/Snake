#pragma once
#include "Input.h"
#include "Map.h"

enum class Direction
{
	UP, DOWN, LEFT, RIGHT
};

struct State {
	bool dangerUp;
	bool dangerDown;
	bool dangerLeft;
	bool dangerRight;
	bool dangerUp;
	bool dangerUp;
	bool dangerUp;
	bool dangerUp;
};

class Game
{
public:
	Game(Map* map, int speed);
	~Game();

public:
	void Update();
	double Movement(Direction action, State state);
	bool HasBodyCollided();
	bool IsOutOfBounds();
	void Reset();
	Direction GetDirection() { return dir; }
	void MoveSnake();
	State GetCurrentState();
	double calculateReward(State newState, State oldState) {
		double reward = 0.0;
		// Calculate distance change to the apple
		int oldDist = oldState.distanceToApple;
		int newDist = newState.distanceToApple;
		int distChange = oldDist - newDist;
		// Positive reward for getting closer to the apple
		reward += distChange * 10.0;
		// Negative reward for moving away from the apple
		reward -= (distChange < 0) ? 10.0 : -10.0;
		// Additional rewards/punishments can be added based on other factors
		return reward;
	}

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

