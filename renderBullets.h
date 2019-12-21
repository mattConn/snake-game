#pragma once

#include <vector>
#include "global.h"
#include "gameObj.h"

void renderBullets(std::vector<gameObj> &bullets)
{
	for (int i = 0; i < bullets.size(); i++)
	{
		// translate
		bullets[i].incRectY(bullets[i].getVelocity());

		// remove bullet if offscreen
		if (bullets[i].isOffscreen())
			bullets.erase(bullets.begin() + i);
		else
			//render bullet
			global::render(bullets[i].getCurrentTexture(), bullets[i].getRectPtr());
	}
}