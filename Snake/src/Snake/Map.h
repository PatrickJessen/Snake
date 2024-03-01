#pragma once
#include "Field.h"
#include <vector>

struct Snake
{
	int X;
	int Y;

	Snake(int x, int y)
		: X(x), Y(y) {}
};

struct Apple
{
	int X;
	int Y;
};

enum class MapType
{
	NONE = 0,
	SNAKE = 1,
	APPLE = 2
};
static const int width = 20;
static const int height = 10;

class Map
{
public:
	Map();
	~Map();

public:
	void Init();
	int GetSize() { return size; }
	std::vector<Snake*> GetSnake() { return snake; }
	Apple GetApple() { return apple; }
	void SetAppleXY(int x, int y);
	void AddSnake();
	int field[width][height];
	int GetScore() { return score; }
	void AddScore();
	void ResetScore() { score = 0; }

private:
	void CreateMap();
	void CreateSnake();
	void CreateApple();

private:
	const int size = 24;

private:
	std::vector<Snake*> snake;
	Apple apple;
	int score;
};