#include "SDLw.h"
#include "game.h"
#include "useful.h"

#include "debug.h"
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace game {

const Uint8 SCREEN_WINDOWED = 0;
const Uint8 SCREEN_FULL = SDL_WINDOW_FULLSCREEN;
Uint8 screenMode = SCREEN_WINDOWED;

const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = SCREEN_WIDTH;

const int SQUARE = SCREEN_WIDTH == SCREEN_HEIGHT ? SCREEN_WIDTH / 16 : 0;

std::map<std::string, SDL_Texture*> allTextures;


// functions
// =========


} // end namespace
