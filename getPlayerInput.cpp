#include "global.h"
#include "gameObj.h"
#include <SDL2/SDL.h>

void getPlayerInput(gameObj& player, const Uint8* keyState)
{
	static bool wasPressed = false;

	// player keybindings
	// ==================

	if (keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_RIGHT])
	{

		// if last move was left/right, ignore left/right inputs
		if(!wasPressed)
		{
			if(player.lastMove == global::LEFT || player.lastMove == global::RIGHT)
			{
				// move up
				if (keyState[SDL_SCANCODE_UP])
					player.lastMove = global::UP;

				// move down
				if (keyState[SDL_SCANCODE_DOWN])
					player.lastMove = global::DOWN;
			}
			else
			{
				// move left
				if (keyState[SDL_SCANCODE_LEFT])
					player.lastMove = global::LEFT;

				// move right
				if (keyState[SDL_SCANCODE_RIGHT])
					player.lastMove = global::RIGHT;
			}
		}

		wasPressed = true;
	}
	else
		wasPressed = false;
}
