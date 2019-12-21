#pragma once

#include "global.h"
#include "gameObj.h"
#include "bulletContainers.h"
#include <SDL2/SDL.h>

void getPlayerInput(gameObj& player, const Uint8* keyState)
{
	enum Move {LEFT, RIGHT, UP, DOWN};

	static int lastMove = LEFT;

	// player keybindings
	// ==================

	// if last move was left/right, ignore left/right inputs
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

	switch(lastMove)
	{
		case UP:
			if(player.getRectTop() > 0)
				player.incRectY(-player.getVelocity() * player.getVelocityMod());
		break;

		case DOWN:
			if(player.getRectBottom() < global::SCREEN_HEIGHT)
				player.incRectY(player.getVelocity() * player.getVelocityMod());
		break;

		case LEFT:
			if(player.getRectL() > 0)
				player.incRectX(-player.getVelocity() * player.getVelocityMod());
		break;

		case RIGHT:
			if(player.getRectR() < global::SCREEN_WIDTH)
				player.incRectX(player.getVelocity() * player.getVelocityMod());
		break;
	}
}
