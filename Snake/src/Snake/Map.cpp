#include "Map.h"
#include "time.h"

Map::Map()
{
	Init();
}

Map::~Map()
{
}

void Map::Init()
{
	CreateMap();
	CreateSnake();
	CreateApple();
}

void Map::SetAppleXY(int x, int y)
{
	apple.X = x;
	apple.Y = y;
}

void Map::AddSnake()
{
	snake.push_back(new Snake(snake[snake.size() - 1]->X, snake[snake.size() - 1]->Y));
	/*switch (x)
	{
	case 0:
		break;
	case 1:
		snake.push_back(new Snake(snake[snake.size() - 1]->X, snake[snake.size() - 1]->Y - 1));
		break;
	case 2:
		snake.push_back(new Snake(snake[snake.size() - 1]->X + 1, snake[snake.size() - 1]->Y));
		break;
	case 3:
		snake.push_back(new Snake(snake[snake.size() - 1]->X - 1, snake[snake.size() - 1]->Y));
		break;
	default:
		break;
	}*/
}

void Map::CreateMap()
{
	for (int i = 0; i < width; i++)
		for (int y = 0; y < height; y++)
		{
			field[i][y] = (int)MapType::NONE;
		}
}

void Map::AddScore()
{
	score += 10;
}

void Map::SetHighScore()
{
	highScore = score;
}

void Map::CreateSnake()
{
	for (int i = 0; i < snake.size(); i++)
		delete snake[i];

	snake.clear();
	snake.push_back(new Snake(rand() % width, rand() % height));
	field[snake[0]->X][snake[0]->Y] = (int)MapType::SNAKE;
}

void Map::CreateApple()
{
	srand(time(0));
	apple.X = rand() % width;
	apple.Y = rand() % height;

	field[apple.X][apple.Y] = (int)MapType::APPLE;
}
