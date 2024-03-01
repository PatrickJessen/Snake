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
	void Update();
	Map* GetMap() { return map; }
	void SetHighScore(int score);
	int GetHighestScore();

private:
	void DrawMap();
	void DrawSnake();
	void DrawApple();
	void InitText();
	void PrintScore();
	void PrintHightScore();
	std::string GetHighScore();

private:
	Window* window;
	Map* map;

private:
	TTF_Font* font;
	SDL_Texture* tex;
	SDL_Surface* tempSurface;
	const char* fontPath;
	int highScore;
	std::string scoreText;
};

