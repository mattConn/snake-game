#include "global.h"
#include "gameObj.h"
#include "animation.h"
#include "bulletContainers.h"

namespace animation {
	bool endAnimation(const gameObj* g)
	{
		if (g->isOffscreen())
			return false;
		else
			return true;
	}
	
	bool wait(gameObj* g)
	{
		return true;
	}

	bool fire(gameObj* g)
	{
		if (SDL_TICKS_PASSED(SDL_GetTicks(), g->getTimeout()))
		{
			currentEnemyBullets.push_back(g->getBulletCopy());
			g->resetTimeout();
		}
		return true;
	}

	bool up(gameObj *g)
	{
		g->decRectY(g->getVelocity());
	
		return true;
	}

	bool down(gameObj *g)
	{
		g->incRectY(g->getVelocity());
	
		return true;
	}

	bool left(gameObj *g)
	{
		g->decRectX(g->getVelocity());
	
		return true;
	}

	bool right(gameObj *g)
	{
		g->incRectX(g->getVelocity());
	
		return true;
	}

	bool downAndLeft(gameObj *g)
	{

		if (abs(g->getInitialY() - g->getRectY()) < 100)
			g->incRectY(5);
		else
			g->decRectX(5);

		return true;
	}

	bool blink(gameObj* g)
	{
		if (SDL_GetTicks() & 1) // render on odd tick (blink)
			global::render(g->getCurrentTexture(), g->getRectPtr());

		return true;
	}
}
