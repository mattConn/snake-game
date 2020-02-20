#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cassert>
#include <string>
#include <list>
#include "SDLw.h"
#include "game.h"

#include "debug.h"

class movePoint {
	public:

	movePoint(){};
	movePoint(int X, int Y, int M) : x(X), y(Y), move(M){};
	
	int x = 0;
	int y = 0;
	int move = 0;	
};

// any game object
// ===============
class gameObj {
	public:

	std::string textureString;
	SDLw::rect rect; // obj rect (used for coordinates)

	double velocity = 1;

	// starting position
	int initialX = 0;
	int initialY = 0;

	bool isOffscreen() const;


	void setInitialPos(const int& x, const int& y)
	{
		initialX = x;
		initialY = y;
	}


	// default constructor
	gameObj();

	// detailed constructor
	// takes texture string, velocity, width, height, xPos, yPos
	gameObj(std::string t, const double& vel, const int &width, const int &height, const int &xPos = 0 ,const int &yPos = 0);

	// copy constructor with rect coords
	// takes rhs gameObj, xPos, yPos
	gameObj(const gameObj& other, const int &xPos, const int &yPos);

	int move;

	// fire rate members
	int duration = 0; // duration of old bullet on screen
	int timeout = 0; // time before bullet shoul be fired

	double velocityMod = 1;

	// sequence of coords and moves
	// ((x,y), move)
	std::list<movePoint> moveSeq;

	void resetTimeout() { timeout = duration + SDL_GetTicks(); }

};
