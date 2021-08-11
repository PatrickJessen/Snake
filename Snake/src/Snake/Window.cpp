#include "Window.h"
#include "Input.h"

Window::Window(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	this->width = width;
	this->height = height;

	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Initialized" << "\n";

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
		{
			std::cout << "Window created" << "\n";
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
			std::cout << "renderer created" << "\n";
		}
		isRunning = true;
	}
	else
	{
		isRunning = false;
	}

	Update();
}

Window::~Window()
{
}

void Window::HandleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
	Input.Update();
}

void Window::Update()
{
	SDL_RenderPresent(renderer);
	HandleEvents();
}

void Window::Clear()
{
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect rect = { 0, 0, width, height };

	SDL_RenderFillRect(renderer, &rect);
}

void Window::Render()
{
	SDL_RenderClear(renderer);
}

void Window::Clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned" << "\n";
}

SDL_Renderer* Window::GetRender()
{
	return renderer;
}

bool Window::Running()
{
	return isRunning;
}

int Window::GetWidth()
{
	return width;
}

int Window::GetHeight()
{
	return height;
}