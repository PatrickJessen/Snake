#include "Draw.h"
#include <fstream>

Draw::Draw(Window* window)
{
	this->window = window;
	map = new Map();
	InitText();
}

Draw::~Draw()
{
	
}

void Draw::Update()
{
	DrawMap();
	PrintScore();
	PrintHightScore();
	//DrawSnake();
	//DrawApple();
}

void Draw::DrawMap()
{
	SDL_Rect rect;
	for (int i = 0; i < width; i++)
		for (int y = 0; y < height; y++)
		{
			if (map->field[i][y] == (int)MapType::SNAKE)
			{
				SDL_SetRenderDrawColor(window->GetRender(), 0, 255, 0, 255);
				DrawSnake();
			}
			if (map->field[i][y] == (int)MapType::APPLE)
			{
				SDL_SetRenderDrawColor(window->GetRender(), 255, 0, 0, 255);
				rect = { map->GetApple().X * map->GetSize(), map->GetApple().Y * map->GetSize(), map->GetSize(), map->GetSize() };
				SDL_RenderFillRect(window->GetRender(), &rect);
			}
			if (map->field[i][y] == (int)MapType::NONE)
			{
			}

		}
	SDL_SetRenderDrawColor(window->GetRender(), 0, 255, 50, 255);
	rect = { 0, 0, width * map->GetSize(), height * map->GetSize() };
	SDL_RenderDrawRect(window->GetRender(), &rect);
}

void Draw::DrawSnake()
{
	for (int x = 0; x < map->GetSnake().size(); x++)
	{
		SDL_Rect rect = { map->GetSnake()[x]->X * map->GetSize(), map->GetSnake()[x]->Y * map->GetSize(), map->GetSize(), map->GetSize() };
		SDL_RenderFillRect(window->GetRender(), &rect);
	}
}

void Draw::DrawApple()
{
	for (int i = 0; i < width; i++)
		for (int y = 0; y < height; y++)
		{
			SDL_SetRenderDrawColor(window->GetRender(), 255, 0, 0, 255);

			SDL_Rect rect = { map->GetApple().X * map->GetSize(), map->GetApple().Y * map->GetSize(), map->GetSize(), map->GetSize() };
			SDL_RenderFillRect(window->GetRender(), &rect);
		}
}

void Draw::InitText()
{
	TTF_Init();
}

void Draw::PrintScore()
{
	SDL_DestroyTexture(tex);
	SDL_FreeSurface(tempSurface);

	scoreText = "Score: " + std::to_string(map->GetScore());
	font = TTF_OpenFont("Assets/arial.ttf", 16);
	SDL_Color color = { 0, 0, 255, 255 };
	tempSurface = TTF_RenderText_Solid(font, scoreText.c_str(), color);
	tex = SDL_CreateTextureFromSurface(window->GetRender(), tempSurface);
	TTF_CloseFont(font);
	SDL_Rect rect = { 1000, 600, 75, 40 };
	SDL_RenderCopy(window->GetRender(), tex, NULL, &rect);
}

void Draw::PrintHightScore()
{
	std::string highScoreText = "Highscore: " + GetHighScore();
	TTF_Font* ttfFont = TTF_OpenFont("Assets/comic.ttf", 20);
	SDL_Color color = { 0, 0, 255, 255 };
	SDL_Surface* surface = TTF_RenderText_Solid(ttfFont, highScoreText.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(window->GetRender(), surface);
	SDL_FreeSurface(surface);
	TTF_CloseFont(ttfFont);
	SDL_Rect rect = { 1000, 200, 75, 40 };
	SDL_RenderCopy(window->GetRender(), texture, NULL, &rect);
	SDL_DestroyTexture(texture);
}

std::string Draw::GetHighScore()
{
	std::string line;
	std::ifstream myfile("Assets/Scores.txt");
	int currentNum = 0;
	int highScore = 0;

	while (std::getline(myfile, line))
	{
		currentNum = std::stoi(line);
		if (highScore < currentNum)
		{
			highScore = currentNum;
		}
	}
	std::string result = std::to_string(highScore).c_str();
	return result;
}
