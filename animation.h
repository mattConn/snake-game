#pragma once

#include "global.h"
#include "gameObj.h"
#include "bulletContainers.h"

namespace animation {
	bool endAnimation(const gameObj* g);
	
	bool wait(gameObj* g);

	bool fire(gameObj* g);

	bool up(gameObj *g);

	bool down(gameObj *g);

	bool left(gameObj *g);

	bool right(gameObj *g);

	bool downAndLeft(gameObj *g);

	bool blink(gameObj* g);
}
