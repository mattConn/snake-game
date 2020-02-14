#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cassert>
#include <string>
#include <utility>
#include "SDLw.h"
#include "game.h"
#include "useful.h"

#include "debug.h"

#include "gameObj.h"

// default constructor
gameObj::gameObj()
{
	rect = SDLw::makeRect(0, 0, 1, 1);
};

// detailed constructor
// takes texture string, velocity, width, height, xPos, yPos
gameObj::gameObj(std::string t, const double& vel, const int &width, const int &height, const int &xPos,const int &yPos) 
{
	textureString = t;
	velocity = vel;
	rect = SDLw::makeRect(xPos, yPos, width, height);
	initialX = xPos;
	initialY = yPos;
};

// copy constructor with rect coords
// takes rhs gameObj, xPos, yPos
gameObj::gameObj(const gameObj& other, const int &xPos, const int &yPos)
{
	textureString = other.textureString;
	velocity = other.velocity;
	
	rect = SDLw::makeRect(xPos, yPos, other.rect.w, other.rect.h);
	initialX = rect.x;
	initialY = rect.y;
};

bool gameObj::isOffscreen() const {
	if (getRectR() < 0 || getRectL() > game::SCREEN_WIDTH || getRectTop() > game::SCREEN_HEIGHT || getRectBottom() < 0)
		return true;
	return false;
};
