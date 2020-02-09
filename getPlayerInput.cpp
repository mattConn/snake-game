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

	switch(player.lastMove)
	{
		case global::UP:
			player.currentTexture = "head-up";
			if(player.getRectTop() > 0)
				player.rect.y += (-player.velocity * player.velocityMod);
		break;

		case global::DOWN:
			player.currentTexture = "head-down";
			if(player.getRectBottom() < global::SCREEN_HEIGHT)
				player.rect.y += (player.velocity * player.velocityMod);
		break;

		case global::LEFT:
			player.currentTexture = "head-left";
			if(player.getRectL() > 0)
				player.rect.x += (-player.velocity * player.velocityMod);
		break;

		case global::RIGHT:
			player.currentTexture = "head-right";
			if(player.getRectR() < global::SCREEN_WIDTH)
				player.rect.x += (player.velocity * player.velocityMod);
		break;
	}
}
