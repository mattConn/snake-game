#include <iostream>
#include <vector>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <ctime>
#undef main

#include "debug.h"
#include "global.h"
#include "gameObj.h"
#include "updateObjPos.h"

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
	global::allTextures["head-up"] = global::loadTexture("head-up.png");
	global::allTextures["head-right"] = global::loadTexture("head-right.png");
	global::allTextures["head-down"] = global::loadTexture("head-down.png");
	global::allTextures["head-left"] = global::loadTexture("head-left.png");
	global::allTextures["body"] = global::loadTexture("body.png");
	global::allTextures["food"] = global::loadTexture("food.png");


	// make player 
	// ===========

	// construct player
	gameObj player = gameObj("head-up", 5, 25, 25, global::SCREEN_WIDTH / 2 - 10 / 2, global::SCREEN_HEIGHT / 2 - 100 / 2);
	player.lastMove = global::UP;

	
	// snake body block to be cloned
	gameObj bodyBlock = gameObj("body", player.velocity, 25, 25, 0, 0);

	// construct food
	gameObj food = gameObj("food", 0, 25, 25, 0, 0);
	food.rect.x = global::randomInt(800 - food.rect.w, food.rect.w);
	food.rect.y = global::randomInt(600 - food.rect.h, food.rect.h);

	// snake body blocks
	std::vector<gameObj> snakeBody;

	// set background
	gameObj bg = gameObj("cloud-bg", 5, 800, 600);

	// game state booleans
	bool quit = false;
	bool paused = false;

	// scorekeeping
	int score = 0;
	int highScore = 0;

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

			// update player pos
			updateObjPos(player);

			// set player texture
			switch(player.lastMove)
			{
				case global::UP:
					player.currentTexture = "head-up";
				break;

				case global::DOWN:
					player.currentTexture = "head-down";
				break;

				case global::LEFT:
					player.currentTexture = "head-left";
				break;

				case global::RIGHT:
					player.currentTexture = "head-right";
				break;
			}

			// collect food
			if(SDL_HasIntersection(&player.rect, &food.rect))
			{
				score++;
				highScore = score > highScore ? score : highScore; // record new highscore

				// increase snake body
				bodyBlock.lastMove = player.lastMove;
				switch(player.lastMove) // place snake block behind head
				{
					case global::UP:
						bodyBlock.rect.x = player.getRectL();
						bodyBlock.rect.y = player.getRectBottom();
					break;

					case global::DOWN:
						bodyBlock.rect.x = player.getRectL();
						bodyBlock.rect.y = player.getRectTop() - player.rect.h;
					break;

					case global::LEFT:
						bodyBlock.rect.x = player.getRectR();
						bodyBlock.rect.y = player.getRectTop();
					break;

					case global::RIGHT:
						bodyBlock.rect.x = player.getRectL() - player.rect.w;
						bodyBlock.rect.y = player.getRectTop();
					break;
				}

				snakeBody.push_back(bodyBlock);

				// new food position
				food.rect.x = global::randomInt(800) - food.rect.w;
				food.rect.y = global::randomInt(600) - food.rect.h;
			} // end food intersection routine

			// screen edge collision
			if(player.getRectL() <= 0 || player.getRectR() >= global::SCREEN_WIDTH || player.getRectTop() <= 0 || player.getRectBottom() >= global::SCREEN_HEIGHT)
			{
				playerIsDead = true;

				DEBUG_MSG("Highscore: " << highScore);

				score = 0; // reset score
			}

			// update snake block position
			for(auto &b : snakeBody)
				updateObjPos(b);

			// render player
			global::render(player.currentTexture, &player.rect);

			// render food 
			global::render(food.currentTexture, &food.rect);

			// render snake body
			for(auto &b : snakeBody)
				global::render(b.currentTexture, &b.rect);
			
		}
		else
		{
			// remove all body blocks
			snakeBody.clear();

			// reset position
			player.rect.x = player.initialX;
			player.rect.y = player.initialY;

			// new food position
			food.rect.x = global::randomInt(800 - food.rect.w, food.rect.w);
			food.rect.y = global::randomInt(600 - food.rect.h, food.rect.h);

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

	DEBUG_MSG("Score: " << score);
	DEBUG_MSG("Highscore: " << highScore);

	return 0;
}
