#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#undef main

int main(int argc, char* argv[])
{
	// init sdl
	if (SDL_Init(SDL_INIT_VIDEO) < 0) return 's';
	if (!IMG_Init(IMG_INIT_PNG)) return 'i';

	// screen dimensions
	#define SCREEN_W 320
	#define SCREEN_H 320

	// create window
	SDL_Window *window = SDL_CreateWindow("Snake",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_W,SCREEN_H,SDL_WINDOW_SHOWN);
	if(!window) return 'w';

	// init renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(!renderer) return 'r';
	SDL_SetRenderDrawColor(renderer,0x00,0x00,0x00,0x00);

	// set window surface
	SDL_Surface *windowSurface = SDL_GetWindowSurface(window);
	// end init


	// hide cursor
	SDL_ShowCursor(SDL_DISABLE);

	#define NUM_TEXTURES 16
	// game textures
	SDL_Texture *textures[NUM_TEXTURES] = {
		IMG_LoadTexture(renderer, "assets/head-up.png"),
		IMG_LoadTexture(renderer, "assets/head-down.png"),
		IMG_LoadTexture(renderer, "assets/head-left.png"),
		IMG_LoadTexture(renderer, "assets/head-right.png"),
		IMG_LoadTexture(renderer, "assets/body.png"),
		IMG_LoadTexture(renderer, "assets/food.png"),
		IMG_LoadTexture(renderer, "assets/0.png"),
		IMG_LoadTexture(renderer, "assets/1.png"),
		IMG_LoadTexture(renderer, "assets/2.png"),
		IMG_LoadTexture(renderer, "assets/3.png"),
		IMG_LoadTexture(renderer, "assets/4.png"),
		IMG_LoadTexture(renderer, "assets/5.png"),
		IMG_LoadTexture(renderer, "assets/6.png"),
		IMG_LoadTexture(renderer, "assets/7.png"),
		IMG_LoadTexture(renderer, "assets/8.png"),
		IMG_LoadTexture(renderer, "assets/9.png")
	};
	for(int i=0;i<NUM_TEXTURES;i++) if(!textures[i]) return 't';


	// make player 
	// ===========

/*
	// construct player
	gameObj player = gameObj("head-up", 5, game::SQUARE, game::SQUARE, game::SCREEN_WIDTH / 2 - 10 / 2, game::SCREEN_HEIGHT / 2 - 100 / 2);
	player.lastMove = game::UP;

	std::vector<gameObj> scoreObjs;
	std::vector<gameObj> highscoreObjs;

	for(int i = 0; i < 3; i++)
	{
		scoreObjs.push_back( gameObj(std::to_string(0), 0, game::SQUARE/2, game::SQUARE/2, i * (game::SQUARE/2), 0) );
		highscoreObjs.push_back( gameObj(std::to_string(0), 0, game::SQUARE/2, game::SQUARE/2, i*(game::SQUARE/2) + game::SCREEN_WIDTH - (game::SQUARE/2)*3, 0) );
	}

	
	// snake body block to be cloned
	// =============================
	// S body block needs a lastMove and a moveSeq.
	// It will move in the direction of lastmove until it reaches the x,y in moveSeq.front().
	// Once it reahes that x,y, it's lastMove will be updated to the move in moveSeq.front().
	// That pair in moveSeq will then be popped off moveSeq front.

	gameObj bodyBlock = gameObj("body", player.velocity, game::SQUARE, game::SQUARE, 0, 0);

	// snake body blocks
	std::vector<gameObj> snakeBody;

	// construct food
	gameObj food = gameObj("food", 0, game::SQUARE, game::SQUARE, 0, 0);

	// set background
	gameObj bg = gameObj("cloud-bg", 5, 800, 600);

	// game state booleans
	bool quit = false;
	bool paused = false;

	// scorekeeping
	int score = 0;
	int highscore = 0;

	// event handler
	SDL_Event event;

	// realtime keystate
	const Uint8* keyState = SDL_GetKeyboardState(nullptr);

	// player life state bools
	bool playerIsDead = true;
	int playerDeathTimeout = 0;
	bool playerDeathRoutineRan = false; // for running death routine only once

	DEBUG_MSG("entering game loop");
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
		SDL_RenderClear(SDLw::renderer);

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
						case game::UP:
							if(xy.second >= b.rect.y)
							{
								b.lastMove = b.moveSeq.front().second;
								b.moveSeq.pop_front();
							}
						break;
						case game::DOWN:
							if(xy.second <= b.rect.y)
							{
								b.lastMove = b.moveSeq.front().second;
								b.moveSeq.pop_front();
							}
						break;
						case game::LEFT:
							if(xy.first >= b.rect.x)
							{
								b.lastMove = b.moveSeq.front().second;
								b.moveSeq.pop_front();
							}
						break;
						case game::RIGHT:
							if(xy.first <= b.rect.x)
							{
								b.lastMove = b.moveSeq.front().second;
								b.moveSeq.pop_front();
							}
						break;

					}
				} // end if moveSeq not empty
				
			}

			// set player texture
			switch(player.lastMove)
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
				bodyBlock.lastMove = snakeBody.back().lastMove;
				bodyBlock.moveSeq = snakeBody.back().moveSeq;

				glueToBack(snakeBody.back(), bodyBlock);

				snakeBody.push_back(bodyBlock);

				// new food position
				food.rect.x = useful::randomInt(game::SCREEN_WIDTH - food.rect.w);
				food.rect.y = useful::randomInt(game::SCREEN_HEIGHT - food.rect.h);
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
				player.lastMove = game::UP;

				// reset snake body
				snakeBody.clear();
				bodyBlock.lastMove = player.lastMove;
				bodyBlock.moveSeq.clear();
				glueToBack(player, bodyBlock);
				snakeBody.push_back(bodyBlock);

				// new food position
				food.rect.x = useful::randomInt(game::SCREEN_WIDTH - food.rect.w);
				food.rect.y = useful::randomInt(game::SCREEN_HEIGHT - food.rect.h);

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

		SDL_Delay(16);
	}

	//==============
	// end game loop
*/

	// clean up

    //Deallocate windowSurface
    SDL_FreeSurface(windowSurface);
    windowSurface = NULL;

    //Destroy window
    SDL_DestroyWindow(window);
    window = NULL;

    // Destroy renderer
    SDL_DestroyRenderer(renderer);

    // Destroy textures
	for(int i=0;i<NUM_TEXTURES;i++) SDL_DestroyTexture(textures[i]);

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();

	return 0;
}
