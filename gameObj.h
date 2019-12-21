#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cassert>
#include <string>
#include <vector>
#include <utility>
#include "baseObjects.h"
#include "global.h"
#include "debug.h"

class gameObj;
typedef std::vector<bool (*)(gameObj*)> animVector;
typedef std::pair<animVector, int> animPair;

// any game object
// ===============
class gameObj {
private:

	std::string currentTexture;

	std::string bullet = "";

	// fire rate members
	int duration = 0; // duration of old bullet on screen
	int timeout = 0; // time before bullet shoul be fired

	SDL_Rect rect; // obj rect (used for coordinates)
	double velocity = 1;
	double velocityMod = 1;

	// starting position
	int initialX = 0;
	int initialY = 0;

	// sequence of animations: pair of vector and duration
	std::vector<animPair> animationSequence;

public:
	// default constructor
	gameObj()
	{
		rect = global::makeRect(0, 0, 1, 1);
	}

	// detailed constructor
	// takes texture string, velocity, width, height, xPos, yPos, bullet string, bullet duration, animation list
	gameObj(std::string t, const double& vel, const int &width, const int &height, const int &xPos = 0 ,const int &yPos = 0, std::string bull = "", const int& dur = 0, const std::initializer_list<animPair> &seq = {}) : currentTexture(t), velocity(vel), bullet(bull), duration(dur), animationSequence(seq)
	{
		rect = global::makeRect(xPos, yPos, width, height);
		setInitialX(xPos);
		setInitialY(yPos);
	}

	// copy constructor with rect coords
	// takes rhs gameObj, xPos, yPos, animation sequence
	gameObj(const gameObj& other, const int &xPos, const int &yPos, const std::vector<animPair> &seq = {}) : currentTexture(other.currentTexture), velocity(other.velocity), bullet(other.bullet), animationSequence(seq)
	{
		rect = global::makeRect(xPos, yPos, other.rect.w, other.rect.h);
		initialX = rect.x;
		initialY = rect.y;
		duration = other.duration;
	}

	// accessors
	std::string getCurrentTexture() const
	{
		return currentTexture;
	}

	void setCurrentTexture(std::string t)
	{
		currentTexture = t;
	}

	void addAnimationSet(const std::vector<bool (*)(gameObj*)> &set, const int &distance = 0)
	{
		animationSequence.push_back({set, distance});
	}

	void playAnimations()
	{
		if (animationSequence.size() == 0) return;

		auto currentRow = animationSequence.front(); // row of animation, distance pair

		// if no duration specified or distance traveled < distance needed, play animations
		if (currentRow.second <= 0 || (abs(rect.x - initialX) < currentRow.second && abs(rect.y - initialY) < currentRow.second) )
		{
			for (auto& anim : currentRow.first)
				anim(this);
		}
		else // erase row, reset initial coords
		{
			animationSequence.erase(animationSequence.begin());
			initialX = rect.x;
			initialY = rect.y;
		}
	}

	bool isOffscreen() const {
		if (getRectR() < 0 || getRectL() > global::SCREEN_WIDTH || getRectTop() > global::SCREEN_HEIGHT || getRectBottom() < 0)
			return true;
		return false;
	}

	// get velocity
	double getVelocity() const { return velocity; }
	double getVelocityMod() const { return velocityMod; }

	void setVelocityMod(const double& v) { velocityMod = v; }

	// get rect sides
	int getRectTop() const { return rect.y; }
	int getRectBottom() const { return rect.y + rect.h; }
	int getRectL() const { return rect.x; }
	int getRectR() const { return rect.x + rect.w; }
	int getRectMiddle() const { return getRectR() - getRectW() / 2; }

	int getRectY() const { return rect.y; }
	int getRectX() const { return rect.x; }
	int getRectW() const { return rect.w; }
	int getRectH() const { return rect.h; }

	void setRectX(const int &x) { rect.x = x; }
	void setRectY(const int &y) { rect.y = y; }
	void setRectW(const int &w) { rect.w = w; }
	void setRectH(const int &h) { rect.h = h; }

	SDL_Rect *getRectPtr() { return &rect; }

	// get initial position
	int getInitialX() const { return initialX; }
	int getInitialY() const { return initialY; }
	
	// set initial position
	void setInitialX(const int& x) { initialX = x; };
	void setInitialY(const int& y) { initialY = y; };

	void setInitialPos(const int& x, const int& y)
	{
		setInitialX(x);
		setInitialY(y);
	}

	// get fire rate info
	int getDuration() const { return duration; }
	int getTimeout() const { return timeout; }

	// set fire rate info
	void setDuration(const int& d) { duration = d; }
	void resetTimeout() { timeout = duration + SDL_GetTicks(); }

	// get bullet
	gameObj getBulletCopy()
	{ 
		assert(bullet != "");
		gameObj newBullet = baseBullets[bullet]; // copy of bullet
		newBullet.setRectX(getRectX() + getRectW()/2 - 8); // center bullet
		newBullet.setRectY(getRectY());

		return newBullet;
	}

	// set bullet
	void setBulletProps(std::string bull, const int& d)
	{
		bullet = bull;
		duration = d;
		timeout = d + SDL_GetTicks();
	}

	// mutators
	void incRectX(const int n) { rect.x += n; }
	void incRectY(const int n) { rect.y += n; }

	void decRectX(const int n) { rect.x -= n; }
	void decRectY(const int n) { rect.y -= n; }

};
