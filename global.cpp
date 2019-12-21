#include "global.h"
#include "debug.h"
#include "baseObjects.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace global {

const Uint8 SCREEN_WINDOWED = 0;
const Uint8 SCREEN_FULL = SDL_WINDOW_FULLSCREEN;
Uint8 screenMode = SCREEN_WINDOWED;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window *window = nullptr; // main window
SDL_Surface *windowSurface = nullptr; // surface for main window
SDL_Renderer *renderer = nullptr; // main renderer

std::map<std::string, SDL_Texture*> allTextures;


// functions
// =========

// SDL rect wrapper
SDL_Rect makeRect(const int &xPos, const int &yPos, const int &width, const int &height)
{
	SDL_Rect rect;
	rect.x = xPos;

	rect.y = yPos;
	rect.w = width;
	rect.h = height;

	return rect;
}

bool render(const std::string texture, const SDL_Rect *rect)
{
	if (SDL_RenderCopy(global::renderer, global::allTextures[texture], nullptr, rect) == 0)
		return true;
	else
		return false;
}

bool init(SDL_Window *&window, SDL_Surface *&windowSurface)
{
	DEBUG_MSG("** Begin init **");

	// init video
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		DEBUG_MSG("SDL could not init video: " << SDL_GetError());
		return false;
	}
	DEBUG_MSG("Init video");

	// init PNG loading
	if (!IMG_Init(IMG_INIT_PNG))
	{
		DEBUG_MSG("Could not init PNG loading: " << SDL_GetError());
		return false;
	}
	DEBUG_MSG("Init PNG loading");

	// create window
	window = SDL_CreateWindow("Bullet Hell",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	if (window == nullptr)
	{
		DEBUG_MSG("SDL window creation error: " << SDL_GetError());
		return false;
	}
	DEBUG_MSG("Created window: w: " << SCREEN_WIDTH << " h: " << SCREEN_HEIGHT );

	// init renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr)
	{
		DEBUG_MSG("Could not init renderer: " << SDL_GetError());
		return false;
	}
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	DEBUG_MSG("Init renderer");

	// assign window surface
	windowSurface = SDL_GetWindowSurface(window);

	DEBUG_MSG("** End init **");

	return true;
}

SDL_Surface *loadImage(const char fileName[])
{
	// load image
	SDL_Surface *imageSurface = IMG_Load(fileName);
	SDL_Surface *optimizedSurface = nullptr;

	if (imageSurface == nullptr)
	{
		DEBUG_MSG("Unable to load image " << fileName << ": " << SDL_GetError());
		return nullptr;
	}

	// optimize image
	optimizedSurface = SDL_ConvertSurface(imageSurface, windowSurface->format, 0);
	if (optimizedSurface == nullptr)
	{
		DEBUG_MSG("Unable to optimize surface " << fileName << ": " << SDL_GetError());
		return nullptr;
	}

	// free unoptimized surface
	SDL_FreeSurface(imageSurface);

	DEBUG_MSG("Load image successful: " << fileName);

	return optimizedSurface;
}

// IMG_LoadTexture wrapper
SDL_Texture *loadTexture(const char fileName[])
{
	SDL_Texture *texture = IMG_LoadTexture(renderer, fileName);

	if (texture == nullptr)
	{
		DEBUG_MSG("Unable to load texture: " << fileName << " : " << SDL_GetError());
		return nullptr;
	}

	DEBUG_MSG("Load texture successful: " << fileName);

	return texture;
}

bool close()
{
	//Deallocate windowSurface
	SDL_FreeSurface(windowSurface);
	windowSurface = nullptr;

	//Destroy window
	SDL_DestroyWindow(window);
	window = nullptr;

	// Destroy renderer
	SDL_DestroyRenderer(renderer);

	// Destroy textures
	for (auto &texture : allTextures)
		SDL_DestroyTexture(texture.second);

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();

	DEBUG_MSG("Close successful");
	return true;
}

} // end namespace
