#include <iostream>
#include <vector>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#undef main

#include "debug.h"
#include "global.h"
#include "gameObj.h"

#include "getPlayerInput.h"

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
	global::allTextures["head"] = global::loadTexture("head.png");
	global::allTextures["body"] = global::loadTexture("body.png");
	global::allTextures["food"] = global::loadTexture("food.png");


	// make player 
	// ===========

	// construct player
	gameObj player = gameObj("head", 5, 25, 25, global::SCREEN_WIDTH / 2 - 10 / 2, global::SCREEN_HEIGHT / 2 - 100 / 2);

	// construct food
	gameObj food = gameObj("food", 0, 25, 25, 10, 10);

	// set background
	gameObj bg = gameObj("cloud-bg", 5, 800, 600);

	// game state booleans
	bool quit = false;
	bool paused = false;
	int deaths = 0;

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

		// player alive routine
		// ====================
		if (!playerIsDead)
		{
			playerDeathTimeout = SDL_GetTicks() + 800; // keep updating death timeout

			// get input
			getPlayerInput(player, keyState);

			if(player.getRectL() <= 0 || player.getRectR() >= global::SCREEN_WIDTH || player.getRectTop() <= 0 || player.getRectBottom() >= global::SCREEN_HEIGHT)
			{
				playerIsDead = true;
				deaths++;
			}

			// render player
			global::render(player.currentTexture, &player.rect);

			// render food 
			global::render(food.currentTexture, &food.rect);

		}
		else
		{
			// reset position
			player.rect.x = player.initialX;
			player.rect.y = player.initialY;

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

	DEBUG_MSG("Deaths: " << deaths);

	return 0;
}
