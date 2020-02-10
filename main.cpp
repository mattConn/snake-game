#include <iostream>
#include <list>
#include <utility>
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
#include "glueToBack.h"

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
	// =============================
	// S body block needs a lastMove and a moveSeq.
	// It will move in the direction of lastmove until it reaches the x,y in moveSeq.front().
	// Once it reahes that x,y, it's lastMove will be updated to the move in moveSeq.front().
	// That pair in moveSeq will then be popped off moveSeq front.

	gameObj bodyBlock = gameObj("body", player.velocity, 25, 25, 0, 0);

	// snake body blocks
	std::vector<gameObj> snakeBody;

	// construct food
	gameObj food = gameObj("food", 0, 25, 25, 0, 0);

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
	bool playerIsDead = true;
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

			// save player move
			int playerMove = player.lastMove;

			// get input
			getPlayerInput(player, keyState);

			// check for new player move, record move
			if(player.lastMove != playerMove)
			{
				for(auto &b : snakeBody)
					b.moveSeq.push_back({{player.rect.x,player.rect.y}, player.lastMove});
			}

			// update player pos
			updateObjPos(player);

			// update snake block moveSeq 
			for(auto &b : snakeBody)
			{
				if(b.moveSeq.size() > 0)
				{
					// block's coords
					std::pair<int, int> xy = {b.moveSeq.front().first.first, b.moveSeq.front().first.second};

					switch(b.lastMove)
					{
						case global::UP:
							if(xy.second >= b.rect.y)
							{
								b.lastMove = b.moveSeq.front().second;
								b.moveSeq.pop_front();
							}
						break;
						case global::DOWN:
							if(xy.second <= b.rect.y)
							{
								b.lastMove = b.moveSeq.front().second;
								b.moveSeq.pop_front();
							}
						break;
						case global::LEFT:
							if(xy.first >= b.rect.x)
							{
								b.lastMove = b.moveSeq.front().second;
								b.moveSeq.pop_front();
							}
						break;
						case global::RIGHT:
							if(xy.first <= b.rect.x)
							{
								b.lastMove = b.moveSeq.front().second;
								b.moveSeq.pop_front();
							}
						break;

							/*
							if(xy.first == b.rect.x && xy.second == b.rect.y)
							{
								b.lastMove = b.moveSeq.front().second;
								b.moveSeq.pop_front();
							}
							*/
					}
				} // end if moveSeq not empty
				
			}

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
				// ===================

				// set new last block's last move and move sequence to old last block's
				bodyBlock.lastMove = snakeBody.back().lastMove;
				bodyBlock.moveSeq = snakeBody.back().moveSeq;

				/*
				switch(snakeBody.back().lastMove) // place snake block behind tail
				{
					case global::UP:
						bodyBlock.rect.x = snakeBody.back().getRectL();
						bodyBlock.rect.y = snakeBody.back().getRectBottom();
					break;

					case global::DOWN:
						bodyBlock.rect.x = snakeBody.back().getRectL();
						bodyBlock.rect.y = snakeBody.back().getRectTop() - player.rect.h;
					break;

					case global::LEFT:
						bodyBlock.rect.x = snakeBody.back().getRectR();
						bodyBlock.rect.y = snakeBody.back().getRectTop();
					break;

					case global::RIGHT:
						bodyBlock.rect.x = snakeBody.back().getRectL() - player.rect.w;
						bodyBlock.rect.y = snakeBody.back().getRectTop();
					break;
				}
				*/
				glueToBack(snakeBody.back(), bodyBlock);

				snakeBody.push_back(bodyBlock);

				// new food position
				food.rect.x = global::randomInt(global::SCREEN_WIDTH - food.rect.w);
				food.rect.y = global::randomInt(global::SCREEN_HEIGHT - food.rect.h);
			} // end food intersection routine

			// screen edge collision
			if(player.getRectL() <= 0 || player.getRectR() >= global::SCREEN_WIDTH || player.getRectTop() <= 0 || player.getRectBottom() >= global::SCREEN_HEIGHT)
			playerIsDead = true;

			// collision with self
			for(int i = 1; i < snakeBody.size(); i++)
				if(SDL_HasIntersection(&player.rect, &snakeBody[i].rect))
					playerIsDead = true;

			// translate snake body blocks
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
		else // player is dead
		{
			score = 0; // reset score

			// reset position
			player.rect.x = player.initialX;
			player.rect.y = player.initialY;

			// reset last move
			player.lastMove = global::UP;

			// reset snake body
			snakeBody.clear();
			bodyBlock.lastMove = player.lastMove;
			bodyBlock.moveSeq.clear();
			glueToBack(player, bodyBlock);
			snakeBody.push_back(bodyBlock);

			// new food position
			food.rect.x = global::randomInt(global::SCREEN_WIDTH - food.rect.w);
			food.rect.y = global::randomInt(global::SCREEN_HEIGHT - food.rect.h);

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
