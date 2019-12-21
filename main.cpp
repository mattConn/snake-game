#include <iostream>
#include <vector>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#undef main

#include "debug.h"
#include "global.h"
#include "bulletContainers.h"
#include "baseObjects.h"
#include "enemyWaves.h"
#include "animation.h"
#include "gameObj.h"
#include "configFromFile.h"

#include "getPlayerInput.h"
#include "renderEnemies.h"
#include "renderBullets.h"

int main(int argc, char* argv[])
{
	// init sdl
	if (!global::init(global::window, global::windowSurface))
	{
		DEBUG_MSG("Init failed");
		return -1;
	}

	// hide cursor
	SDL_ShowCursor(SDL_DISABLE);

	// containers
	// ==========

	// load textures
	global::allTextures["player"] = global::loadTexture("player.png");


	// make player 
	// ===========

	// construct player
	gameObj player = gameObj("player", 8, 50, 85, global::SCREEN_WIDTH / 2 - 10 / 2, global::SCREEN_HEIGHT / 2 - 100 / 2, "red", 100);

	// set background
	gameObj bg = gameObj("cloud-bg", 5, 800, 600);
	SDL_Rect bgRect = *bg.getRectPtr(); // rect for 2nd bg render
	bgRect.y = -bg.getRectH();

	// game state booleans
	bool quit = false;
	bool paused = false;

	// event handler
	SDL_Event event;

	// realtime keystate
	const Uint8* keyState = SDL_GetKeyboardState(nullptr);

	// player life state bools
	bool playerIsDead = false;
	int playerDeathTimeout;
	

	// game loop
	//===========
	while (!quit)
	{

		// event polling loop
		while (SDL_PollEvent(&event))
		{
			// window close event
			if (event.type == SDL_QUIT)
			{
				quit = true;
				break;
			}

			// keyboard events
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE: // pause
					paused = paused ? false : true;
					break;

				case SDLK_RETURN: // quit
					quit = true;
					break;
				}
			} // end get keyboard events
		} // end poll events


		// skip scene updating when paused
		if (paused) goto renderPresent;

		// update scene
		// ============

		// clear window
		SDL_RenderClear(global::renderer);

		// player alive routine (render player, enemy bullets)
		// ===================================================
		if (!playerIsDead)
		{
			playerDeathTimeout = SDL_GetTicks() + 500; // keep updating death timeout

			// get input
			getPlayerInput(player, keyState);

			// render player
			global::render(player.getCurrentTexture(), player.getRectPtr());

		}
		else
		{
			// player comes back
			if (SDL_TICKS_PASSED(SDL_GetTicks(), playerDeathTimeout))
				playerIsDead = false;
		}

		// render current textures
		renderPresent:

		SDL_RenderPresent(global::renderer);

		SDL_Delay(16);
	}

	//==============
	// end game loop

	// close SDL subsystems
	global::close();

	return 0;
}
