#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <map>

class gameObj;
namespace game {

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

	extern std::map<std::string, SDL_Texture*> allTextures;

	// function prototypes
	// ===================

} // end namespace
