#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <map>

class gameObj;
namespace global {

	extern const Uint8 SCREEN_WINDOWED, SCREEN_FULL;
	extern Uint8 screenMode;

	// screen dimensions
	extern const int SCREEN_HEIGHT;
	extern const int SCREEN_WIDTH;

	extern const int SQUARE;

	// moves
	enum Moves { UP, DOWN, LEFT, RIGHT, MOVES_TOTAL };

	// keypress enum for relating textures to keypress events
	enum KeyPresses
	{
		KEY_PRESS_DEFAULT,
		KEY_PRESS_UP,
		KEY_PRESS_DOWN,
		KEY_PRESS_LEFT,
		KEY_PRESS_RIGHT,
		KEY_PRESS_TOTAL
	};

	extern SDL_Window *window; // main window
	extern SDL_Surface *windowSurface; // surface for main window
	extern SDL_Renderer *renderer; // main renderer

	extern std::map<std::string, SDL_Texture*> allTextures;


	// function prototypes
	// ===================

	// write data to file
	template <class T>
	bool writeToFile(const std::string &fileName, const T &data);

	// return current datestring
	std::string getDateString();

	// generate random int over inclusive range
	int randomInt(const int &max, const int &min = 0);

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
