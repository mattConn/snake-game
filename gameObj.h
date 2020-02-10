#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cassert>
#include <string>
#include <vector>
#include <utility>
#include "global.h"
#include "debug.h"


// any game object
// ===============
class gameObj {
	public:

	std::string currentTexture;

	int lastMove;

	// fire rate members
	int duration = 0; // duration of old bullet on screen
	int timeout = 0; // time before bullet shoul be fired

	SDL_Rect rect; // obj rect (used for coordinates)
	double velocity = 1;
	double velocityMod = 1;

	// starting position
	int initialX = 0;
	int initialY = 0;

	// sequence of coords and moves
	// ((x,y), move)
	std::vector<std::pair<std::pair<int, int>, int>> turningPoints;

	// default constructor
	gameObj();

	// detailed constructor
// takes texture string, velocity, width, height, xPos, yPos
	gameObj(std::string t, const double& vel, const int &width, const int &height, const int &xPos = 0 ,const int &yPos = 0);

	// copy constructor with rect coords
	// takes rhs gameObj, xPos, yPos
	gameObj(const gameObj& other, const int &xPos, const int &yPos);

	bool isOffscreen() const;

	int getRectTop() const { return rect.y; }
	int getRectBottom() const { return rect.y + rect.h; }
	int getRectL() const { return rect.x; }
	int getRectR() const { return rect.x + rect.w; }

	void setInitialPos(const int& x, const int& y)
	{
		initialX = x;
		initialY = y;
	}

	void resetTimeout() { timeout = duration + SDL_GetTicks(); }

};
