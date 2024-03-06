#pragma once
#include "Map.h"
#include "Window.h"
#include "SDL_ttf.h"
#include <string>

class Draw
{
public:
	Draw(Window* window);
	~Draw();

public:
	void Update(Map* map);
	void SetHighScore(int score);
	int GetHighestScore();

private:
	void DrawMap(Map* map);
	void DrawSnake(Map* map);
	void DrawApple(Map* map);
	void InitText();
	void PrintScore(Map* map);
	void PrintHightScore();
	std::string GetHighScore();

private:
	Window* window;

private:
	TTF_Font* font;
	SDL_Texture* tex;
	SDL_Surface* tempSurface;
	const char* fontPath;
	int highScore;
	std::string scoreText;
};

