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

	class rect : public SDL_Rect {
		public:

		rect(){x = y = w = h = 0;}; // default constructor
		rect(const int &X, const int &Y, const int &W, const int &H){ x = X; y = Y; w = W; h = H;};

		void setPos(const int& X, const int& Y){ x = X; y = Y; }
		int getTop() const { return y; }
		int getBottom() const { return y + h; }
		int getL() const { return x; }
		int getR() const { return x + w; }
	
	};

	// function prototypes
	// ===================

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
