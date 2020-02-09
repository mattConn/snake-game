#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cassert>
#include <string>
#include <vector>
#include <utility>
#include "global.h"
#include "debug.h"

#include "gameObj.h"

typedef std::vector<bool (*)(gameObj*)> animVector;
typedef std::pair<animVector, int> animPair;

// default constructor
gameObj::gameObj()
{
	rect = global::makeRect(0, 0, 1, 1);
};

// detailed constructor
// takes texture string, velocity, width, height, xPos, yPos
gameObj::gameObj(std::string t, const double& vel, const int &width, const int &height, const int &xPos,const int &yPos) 
{
	currentTexture = t;
	velocity = vel;
	rect = global::makeRect(xPos, yPos, width, height);
	initialX = xPos;
	initialY = yPos;
};

// copy constructor with rect coords
// takes rhs gameObj, xPos, yPos
gameObj::gameObj(const gameObj& other, const int &xPos, const int &yPos)
{
	currentTexture = other.currentTexture;
	velocity = other.velocity;
	
	rect = global::makeRect(xPos, yPos, other.rect.w, other.rect.h);
	initialX = rect.x;
	initialY = rect.y;
};

bool gameObj::isOffscreen() const {
	if (getRectR() < 0 || getRectL() > global::SCREEN_WIDTH || getRectTop() > global::SCREEN_HEIGHT || getRectBottom() < 0)
		return true;
	return false;
};
