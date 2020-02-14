#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <map>

// SDL wrappers

namespace SDLw {

	extern SDL_Window *window; // main window
	extern SDL_Surface *windowSurface; // surface for main window
	extern SDL_Renderer *renderer; // main renderer

	// function prototypes
	// ===================

	// SDL rect wrapper
	SDL_Rect makeRect(const int &x, const int &y, const int &w, const int &h);

	// SDL_RenderCopy wrapper
	bool render(const std::string texture, const SDL_Rect* rect);

	// init SDL subsystems, windows etc.
	bool init(SDL_Window *&window, SDL_Surface *&windowSurface);

	// load image and optimize
	SDL_Surface *loadImage(char fileName[]);

	// SDL image wrapper
	SDL_Texture *loadTexture(const char filename[]);

	// free memory and quit SDL subsytems
	bool close();

} // end namespace
