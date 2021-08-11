#pragma once
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>

class Window
{

public:
	Window(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	~Window();

	//void Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void HandleEvents();
	void Update();
	void Clear();
	void Render();
	void Clean();
	SDL_Renderer* GetRender();

	bool Running();

public:
	int GetWidth();
	int GetHeight();
	SDL_Renderer* renderer;

private:
	bool isRunning;
	SDL_Window* window;

private:
	int width;
	int height;
};