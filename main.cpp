#include <iostream>
#include <list>
#include <utility>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#undef main

#include "debug.h"
#include "SDLw.h"
#include "game.h"

#include "gameObj.h"
#include "updateObjPos.h"

#include "getPlayerInput.h"
#include "glueToBack.h"

#include "emscripten.h"

gameObj player;

std::vector<gameObj> scoreObjs;
std::vector<gameObj> highscoreObjs;

// snake body block to be cloned
// =============================
// S body block needs a move and a moveSeq.
// It will move in the direction of lastmove until it reaches the x,y in moveSeq.front().
// Once it reahes that x,y, it's move will be updated to the move in moveSeq.front().
// That pair in moveSeq will then be popped off moveSeq front.

gameObj bodyBlock;

std::vector<gameObj> snakeBody;

gameObj food;

// game state booleans
bool quit;
bool paused;

// scorekeeping
int score;
int highscore;

// event handler
SDL_Event event;

// realtime keystate
const Uint8* keyState;

// player life state bools
bool playerIsDead;
int playerDeathTimeout;
bool playerDeathRoutineRan;

// generate random int over inclusive range
int randomInt(const int &max, const int &min = 0)
{
	std::srand(SDL_GetTicks()); 
	return std::rand() % max + min;
}

void mainloop()
{

	// game loop
	//===========
	//while (!quit)
	//{
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
		SDL_RenderClear(SDLw::renderer);

		// player alive routine
		// ====================
		if (!playerIsDead)
		{
			playerDeathTimeout = SDL_GetTicks() + 800; // keep updating death timeout

			// save player move
			int playerMove = player.move;

			// get input
			getPlayerInput(player, keyState);

			// check for new player move, record move
			if(player.move != playerMove)
			{
				for(auto &b : snakeBody)
				{
					b.moveSeq.push_back(movePoint(player.rect.x, player.rect.y, player.move));
				}
			}

			// update player pos
			updateObjPos(player);

			// update snake block moveSeq 
			for(auto &b : snakeBody)
			{
				if(b.moveSeq.size() > 0)
				{
					// block's coords
					std::pair<int, int> xy = {b.moveSeq.front().x, b.moveSeq.front().y};

					switch(b.move)
					{
						case game::UP:
							if(xy.second >= b.rect.y)
							{
								b.move = b.moveSeq.front().move;
								b.moveSeq.pop_front();
							}
						break;
						case game::DOWN:
							if(xy.second <= b.rect.y)
							{
								b.move = b.moveSeq.front().move;
								b.moveSeq.pop_front();
							}
						break;
						case game::LEFT:
							if(xy.first >= b.rect.x)
							{
								b.move = b.moveSeq.front().move;
								b.moveSeq.pop_front();
							}
						break;
						case game::RIGHT:
							if(xy.first <= b.rect.x)
							{
								b.move = b.moveSeq.front().move;
								b.moveSeq.pop_front();
							}
						break;
					}
				} // end if moveSeq not empty
				
			}

			// set player texture
			switch(player.move)
			{
				case game::UP:
					player.textureString = "head-up";
				break;

				case game::DOWN:
					player.textureString = "head-down";
				break;

				case game::LEFT:
					player.textureString = "head-left";
				break;

				case game::RIGHT:
					player.textureString = "head-right";
				break;
			}

			// collect food
			if(SDL_HasIntersection(&player.rect, &food.rect))
			{
				score++;
				highscore = score > highscore ? score : highscore; // record new highscore

				int scoreTmp = score;
				int highscoreTmp = highscore;
				
				// update score textures
				for(int i = scoreObjs.size()-1; i > -1; i--)
				{
					scoreObjs[i].textureString = std::to_string(scoreTmp % 10);
					scoreTmp /= 10;
				}

				// update highscore textures
				for(int i = highscoreObjs.size()-1; i > -1; i--)
				{
					highscoreObjs[i].textureString = std::to_string(highscoreTmp % 10);
					highscoreTmp /= 10;
				}
				

				// increase snake body
				// ===================

				// set new last block's last move and move sequence to old last block's
				bodyBlock.move = snakeBody.back().move;
				bodyBlock.moveSeq = snakeBody.back().moveSeq;

				glueToBack(snakeBody.back(), bodyBlock);

				snakeBody.push_back(bodyBlock);

				// new food position
				food.rect.x = randomInt(game::SCREEN_WIDTH - food.rect.w);
				food.rect.y = randomInt(game::SCREEN_HEIGHT - food.rect.h);
			} // end food intersection routine

			// screen edge collision
			if(player.rect.getL() <= 0 || player.rect.getR() >= game::SCREEN_WIDTH || player.rect.getTop() <= 0 || player.rect.getBottom() >= game::SCREEN_HEIGHT)
			playerIsDead = true;

			// collision with self
			for(int i = 1; i < snakeBody.size(); i++)
				if(SDL_HasIntersection(&player.rect, &snakeBody[i].rect))
					playerIsDead = true;

			// translate snake body blocks
			for(auto &b : snakeBody)
				updateObjPos(b);

			// render score
			for(auto &s : scoreObjs)
				SDLw::render(s.textureString, &s.rect);

			for(auto &s : highscoreObjs)
				SDLw::render(s.textureString, &s.rect);


			// render player
			SDLw::render(player.textureString, &player.rect);

			// render food 
			SDLw::render(food.textureString, &food.rect);

			// render snake body
			for(auto &b : snakeBody)
				SDLw::render(b.textureString, &b.rect);

			
		}
		else // player is dead
		{
			if(!playerDeathRoutineRan)
			{
				score = 0; // reset score

				for(int i = 0; i < scoreObjs.size(); i++)
					scoreObjs[i].textureString = "0";

				// reset position
				player.rect.x = player.initialX;
				player.rect.y = player.initialY;

				// reset last move
				player.move = game::UP;

				// reset snake body
				snakeBody.clear();
				bodyBlock.move = player.move;
				bodyBlock.moveSeq.clear();
				glueToBack(player, bodyBlock);
				snakeBody.push_back(bodyBlock);

				// new food position
				food.rect.x = randomInt(game::SCREEN_WIDTH - food.rect.w);
				food.rect.y = randomInt(game::SCREEN_HEIGHT - food.rect.h);

				playerDeathRoutineRan = true;
			}

			// player comes back
			if (SDL_TICKS_PASSED(SDL_GetTicks(), playerDeathTimeout))
			{
				playerIsDead = false;
				playerDeathRoutineRan = false;
			}
		}

		// render current textures
		renderPresent:

		SDL_RenderPresent(SDLw::renderer);

		//SDL_Delay(16);

		  if(quit) {
		    emscripten_cancel_main_loop();
			SDLw::close();
			
		  }
	//}

	//==============
	// end game loop

	// close SDL subsystems

	//DEBUG_MSG("Score: " << score);
	//DEBUG_MSG("Highscore: " << highscore);

}

int main()
{
	// init sdl
	if (!SDLw::init(SDLw::window, SDLw::windowSurface))
	{
		DEBUG_MSG("Init failed");
		return -1;
	}

	// hide cursor
	SDL_ShowCursor(SDL_DISABLE);

	// containers
	// ==========

	// load textures
	game::allTextures["head-up"] = SDLw::loadTexture("assets/head-up.png");
	game::allTextures["head-right"] = SDLw::loadTexture("assets/head-right.png");
	game::allTextures["head-down"] = SDLw::loadTexture("assets/head-down.png");
	game::allTextures["head-left"] = SDLw::loadTexture("assets/head-left.png");
	game::allTextures["body"] = SDLw::loadTexture("assets/body.png");
	game::allTextures["food"] = SDLw::loadTexture("assets/food.png");
	game::allTextures["0"] = SDLw::loadTexture("assets/0.png");
	game::allTextures["1"] = SDLw::loadTexture("assets/1.png");
	game::allTextures["2"] = SDLw::loadTexture("assets/2.png");
	game::allTextures["3"] = SDLw::loadTexture("assets/3.png");
	game::allTextures["4"] = SDLw::loadTexture("assets/4.png");
	game::allTextures["5"] = SDLw::loadTexture("assets/5.png");
	game::allTextures["6"] = SDLw::loadTexture("assets/6.png");
	game::allTextures["7"] = SDLw::loadTexture("assets/7.png");
	game::allTextures["8"] = SDLw::loadTexture("assets/8.png");
	game::allTextures["9"] = SDLw::loadTexture("assets/9.png");


	// make player 
	// ===========

	// construct player
	player = gameObj("head-up", 5, game::SQUARE, game::SQUARE, game::SCREEN_WIDTH / 2 - 10 / 2, game::SCREEN_HEIGHT / 2 - 100 / 2);
	player.move = game::UP;

	for(int i = 0; i < 3; i++)
	{
		scoreObjs.push_back( gameObj(std::to_string(0), 0, game::SQUARE/2, game::SQUARE/2, i * (game::SQUARE/2), 0) );
		highscoreObjs.push_back( gameObj(std::to_string(0), 0, game::SQUARE/2, game::SQUARE/2, i*(game::SQUARE/2) + game::SCREEN_WIDTH - (game::SQUARE/2)*3, 0) );
	}

	
	// snake body block to be cloned
	// =============================
	// S body block needs a move and a moveSeq.
	// It will move in the direction of lastmove until it reaches the x,y in moveSeq.front().
	// Once it reahes that x,y, it's move will be updated to the move in moveSeq.front().
	// That pair in moveSeq will then be popped off moveSeq front.

	bodyBlock = gameObj("body", player.velocity, game::SQUARE, game::SQUARE, 0, 0);

	// construct food
	food = gameObj("food", 0, game::SQUARE, game::SQUARE, 0, 0);

	// game state booleans
	quit = false;
	paused = false;

	// scorekeeping
	score = 0;
	highscore = 0;

	// realtime keystate
	keyState = SDL_GetKeyboardState(nullptr);

	// player life state bools
	playerIsDead = true;
	playerDeathTimeout = 0;
	playerDeathRoutineRan = false; // for running death routine only once per timeout

	emscripten_set_main_loop(mainloop, 0, 1);
	return 0;
}
