#include "SDLw.h"
#include "game.h"
#include "useful.h"

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
			if(player.lastMove == game::LEFT || player.lastMove == game::RIGHT)
			{
				// move up
				if (keyState[SDL_SCANCODE_UP])
					player.lastMove = game::UP;

				// move down
				if (keyState[SDL_SCANCODE_DOWN])
					player.lastMove = game::DOWN;
			}
			else
			{
				// move left
				if (keyState[SDL_SCANCODE_LEFT])
					player.lastMove = game::LEFT;

				// move right
				if (keyState[SDL_SCANCODE_RIGHT])
					player.lastMove = game::RIGHT;
			}
		}

		wasPressed = true;
	}
	else
		wasPressed = false;
}
