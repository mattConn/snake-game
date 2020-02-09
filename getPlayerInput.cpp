#pragma once

#include "global.h"
#include "gameObj.h"
#include "bulletContainers.h"
#include <SDL2/SDL.h>

void getPlayerInput(gameObj& player, const Uint8* keyState)
{
	enum Move {LEFT, RIGHT, UP, DOWN};

	static int lastMove = LEFT;

	static bool wasPressed = false;

	// player keybindings
	// ==================

	if (keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_RIGHT])
	{

		// if last move was left/right, ignore left/right inputs
		if(!wasPressed)
		{
			if(lastMove == LEFT || lastMove == RIGHT)
			{
				// move up
				if (keyState[SDL_SCANCODE_UP])
					lastMove = UP;

				// move down
				if (keyState[SDL_SCANCODE_DOWN])
					lastMove = DOWN;
			}
			else
			{
				// move left
				if (keyState[SDL_SCANCODE_LEFT])
					lastMove = LEFT;

				// move right
				if (keyState[SDL_SCANCODE_RIGHT])
					lastMove = RIGHT;
			}
		}

		wasPressed = true;
	}
	else
		wasPressed = false;

	switch(lastMove)
	{
		case UP:
			if(player.getRectTop() > 0)
				player.rect.y += (-player.velocity * player.velocityMod);
		break;

		case DOWN:
			if(player.getRectBottom() < global::SCREEN_HEIGHT)
				player.rect.y += (player.velocity * player.velocityMod);
		break;

		case LEFT:
			if(player.getRectL() > 0)
				player.rect.x += (-player.velocity * player.velocityMod);
		break;

		case RIGHT:
			if(player.getRectR() < global::SCREEN_WIDTH)
				player.rect.x += (player.velocity * player.velocityMod);
		break;
	}
}
