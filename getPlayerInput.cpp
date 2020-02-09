#include "global.h"
#include "gameObj.h"
#include "getPlayerInput.h"
#include <SDL2/SDL.h>

void getPlayerInput(gameObj& player, const Uint8* keyState)
{
	enum Moves {LEFT, UP, RIGHT, DOWN};

	static int lastMove = UP;

	static bool wasPressed = false;


	// out of range prevention
	// =======================
	if (lastMove > 3) lastMove = 0; // upper bound
	if (lastMove < 0) lastMove = 3; // lower bound

	// map moves to coordinate changes
	// ===============================
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

	// player keybindings: rotate player
	// =================================

	if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_RIGHT])
	{
		if(!wasPressed)
		{
			if (keyState[SDL_SCANCODE_LEFT])
				lastMove--;
			else if (keyState[SDL_SCANCODE_RIGHT])
				lastMove++;
		}
		
		wasPressed = true;
	}
	else
		wasPressed = false; // reset
}
