#pragma once

#include <vector>
#include "global.h"
#include "gameObj.h"

void renderEnemies(std::vector<gameObj> &enemies, std::vector<gameObj> &bullets)
{
	// render all current objs
			// =======================
	for (int i = 0; i < enemies.size(); i++)
	{
		if (!enemies[i].isOffscreen()) // if not offscreen
		{
			// render
			global::render(enemies[i].getCurrentTexture(), enemies[i].getRectPtr());

			// play animations
			enemies[i].playAnimations();
				
			// check for player bullet collision
			for (int j = 0; j < bullets.size(); j++)
			{
				if (SDL_HasIntersection(enemies[i].getRectPtr(), bullets[j].getRectPtr()))
				{
					enemies.erase(enemies.begin() + i);

					bullets.erase(bullets.begin() + j);
					break; // avoid out of range index
				}
			}
		}
		else // offscreen: remove
		{
			enemies.erase(enemies.begin() + i);
		}
	}
}
