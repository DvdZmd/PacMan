#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "assert.h"
#include "pacman.h"
#include "drawer.h"
#include <iostream>

int main(int argc, char** argv)
{
	/* initialize SDL */

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		assert(0 && "Failed to initialize video!");
		exit(-1);
	}

	SDL_Window* window = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (!window)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		assert(0 && "Failed to create window!");
		exit(-1);
	}

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	if (TTF_Init() == -1)
	{
		SDL_Log("Unable to create ttf: %s", SDL_GetError());
		assert(0 && "Failed to create ttf!");
		exit(-1);
	}

	Drawer* drawer = Drawer::Create(window, renderer);
	Pacman* pacman = Pacman::Create(drawer);

	float lastFrame = (float)SDL_GetTicks() * 0.001f;
	SDL_Event event;

	while (SDL_PollEvent(&event) >= 0)
	{
		//This is for synchronice the engine drawing with system ticks
		float ticks = (float)SDL_GetTicks();//Get the number of milliseconds since SDL library initialization.
		float currentFrame = ticks * 0.001f;//Tranform it to seconds
		float elapsedTime = currentFrame - lastFrame;//calculate empleased time in previous loop

		//Update the world objects and characters properties and propagate interaction
		if (!pacman->Update(elapsedTime))
			break;

		//Starts redrawing operation
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		pacman->Draw();

		lastFrame = currentFrame;

		SDL_RenderPresent(renderer);
		SDL_Delay(1);
	}

	delete pacman;
	delete drawer;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}

