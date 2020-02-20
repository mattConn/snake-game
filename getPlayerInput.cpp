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
			if(player.move == game::LEFT || player.move == game::RIGHT)
			{
				// move up
				if (keyState[SDL_SCANCODE_UP])
					player.move = game::UP;

				// move down
				if (keyState[SDL_SCANCODE_DOWN])
					player.move = game::DOWN;
			}
			else
			{
				// move left
				if (keyState[SDL_SCANCODE_LEFT])
					player.move = game::LEFT;

				// move right
				if (keyState[SDL_SCANCODE_RIGHT])
					player.move = game::RIGHT;
			}
		}

		wasPressed = true;
	}
	else
		wasPressed = false;
}
