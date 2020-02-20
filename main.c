#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#undef main

// constants
#define VELOCITY 5
#define SCREEN_W 320
#define SCREEN_H 320
#define NUM_TEXTURES 16
#define TILE_W 16
#define TILE_H 16
#define EMPTY -1

int main(int argc, char* argv[])
{
	// init sdl
	if (SDL_Init(SDL_INIT_VIDEO) < 0) return 's';
	if (!IMG_Init(IMG_INIT_PNG)) return 'i';

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

	// game textures
	SDL_Texture *textures[NUM_TEXTURES] = {
		IMG_LoadTexture(renderer, "assets/0.png"),
		IMG_LoadTexture(renderer, "assets/1.png"),
		IMG_LoadTexture(renderer, "assets/2.png"),
		IMG_LoadTexture(renderer, "assets/3.png"),
		IMG_LoadTexture(renderer, "assets/4.png"),
		IMG_LoadTexture(renderer, "assets/5.png"),
		IMG_LoadTexture(renderer, "assets/6.png"),
		IMG_LoadTexture(renderer, "assets/7.png"),
		IMG_LoadTexture(renderer, "assets/8.png"),
		IMG_LoadTexture(renderer, "assets/9.png"),
		IMG_LoadTexture(renderer, "assets/head-up.png"),
		IMG_LoadTexture(renderer, "assets/head-down.png"),
		IMG_LoadTexture(renderer, "assets/head-left.png"),
		IMG_LoadTexture(renderer, "assets/head-right.png"),
		IMG_LoadTexture(renderer, "assets/body.png"),
		IMG_LoadTexture(renderer, "assets/food.png")
	};
	for(int i=0;i<NUM_TEXTURES;i++) if(!textures[i]) return 't';

	// textures enum
	typedef enum Texture {HEAD_UP = 10, HEAD_DOWN, HEAD_LEFT, HEAD_RIGHT, BODY, FOOD} texture;

	// moves
	typedef enum Moves {UP, DOWN, LEFT, RIGHT, MAX_MOVES} moves;

	// make player 
	SDL_Rect player;
	player.x = SCREEN_W/2; // 160
	player.y = SCREEN_H/2;
	player.w = TILE_W;
	player.h = TILE_H;
	int playerMove = UP;
	int playerTexture = HEAD_UP;

	SDL_Rect scoreRects[3];
	SDL_Rect highscoreRects[3];

	// position score rects in upper left
	// position highscore rects in upper right
	for(int i = 0; i < 3; i++)
	{
		scoreRects[i].w = TILE_W/2;
		scoreRects[i].h = TILE_H;
		scoreRects[i].y = 0;
		scoreRects[i].x = i*(TILE_W/2);

		highscoreRects[i].w = TILE_W/2;
		highscoreRects[i].h = TILE_H;
		highscoreRects[i].y = 0;
		highscoreRects[i].x = i*(TILE_W/2) + SCREEN_W - (TILE_W/2)*3;
	}

	// snake blocks
	int sbCapacity = 10;
	SDL_Rect *snakeBody = (SDL_Rect*) malloc(sizeof(SDL_Rect) * sbCapacity);
	int sbIndex = 0;

	/*
	// snake blocks	
	SDL_Rect snakeBody[399];
	int sbIndex = 0;

	// snake body move sequence, deref with sbIndex
	// array of arrays of x,y,move
	// each array for one snake block
	// 1000 moves?
	int moveSeq[399][1000][3];
	for(int i=0; i < 399;i++)
	for(int j=0; j < 1000;j++)
	{
		moveSeq[i][j][0] = EMPTY; // x
		moveSeq[i][j][1] = EMPTY; // y
		moveSeq[i][j][2] = EMPTY; // move
	}
	*/

	// construct food
	SDL_Rect food = player;

	// seed random num generation for food pos
	srand(time(NULL));

	// game state booleans
	bool quit = false;
	bool paused = false;

	// scorekeeping
	int score = 0;
	int highscore = 0;

	// event handler
	SDL_Event event;

	// realtime keystate
	const Uint8* keyState = SDL_GetKeyboardState(NULL);

	// player life state bools
	bool playerIsDead = true;
	int playerDeathTimeout = 0;
	bool playerDeathRoutineRan = false; // for running death routine only once per timeout


	// game loop
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

		// clear window
		SDL_RenderClear(renderer);

		// player alive routine
		if (!playerIsDead)
		{
			playerDeathTimeout = SDL_GetTicks() + 800; // keep updating death timeout

			// save player move
			int playerLastMove = playerMove;

			// get input
			static bool wasPressed = false; // press debounce
			if (keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_RIGHT])
			{
				// if last move was left/right, ignore left/right inputs
				if(!wasPressed)
				{
					// move up or down
					if(playerMove == LEFT || playerMove == RIGHT)
						playerMove = keyState[SDL_SCANCODE_UP] ? UP : DOWN;
					// move left or right
					else
						playerMove = keyState[SDL_SCANCODE_LEFT] ? LEFT : RIGHT;
				}

				wasPressed = true;
			}
			else
				wasPressed = false;

			// end get input

		/*
			// check for new player move, record move
			if(playerMove != playerLastMove)
			{
				for(int i=0; i < sbIndex; i++)
				{
					// find next available point
					int j = 0;
					while(moveSeq[sbIndex][j][0] != EMPTY) j++;

					// store x,y,move
					moveSeq[sbIndex][j][0] = player.x;
					moveSeq[sbIndex][j][0] = player.y;
					moveSeq[sbIndex][j][0] = playerMove;
				}
			}
		*/

			// update player pos and texture
			switch(playerMove)
			{
				case UP:
					player.y -= VELOCITY;
					playerTexture = HEAD_UP;
				break;

				case DOWN:
					player.y += VELOCITY;
					playerTexture = HEAD_DOWN;
				break;

				case LEFT:
					player.x -= VELOCITY;
					playerTexture = HEAD_LEFT;
				break;

				case RIGHT:
					player.x += VELOCITY;
					playerTexture = HEAD_RIGHT;
				break;
			}
/*

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
*/

			// collect food
			if(SDL_HasIntersection(&player, &food))
			{
				score++;
				highscore = score > highscore ? score : highscore; // record new highscore

				int scoreTmp = score;
				int highscoreTmp = highscore;

				// increase snake body

				if(sbIndex == sbCapacity)
				{
					sbCapacity+=10;
					SDL_Rect * sbTemp = realloc(snakeBody, sbCapacity*sizeof(SDL_Rect));
					snakeBody = sbTemp;
				}

				(snakeBody+sbIndex)->w = TILE_W;
				(snakeBody+sbIndex)->h = TILE_H;

				// position new block behind player
				switch(playerMove)
				{
					case UP:
					(snakeBody+sbIndex)->x = player.x;
					(snakeBody+sbIndex)->y = player.y+player.h;
					break;

					case DOWN:
					(snakeBody+sbIndex)->x = player.x;
					(snakeBody+sbIndex)->y = player.y;
					break;

					case LEFT:
					(snakeBody+sbIndex)->x = player.x+player.w;
					(snakeBody+sbIndex)->y = player.y;
					break;

					case RIGHT:
					(snakeBody+sbIndex)->x = player.x-player.w;
					(snakeBody+sbIndex)->y = player.y;
					break;
				}

				sbIndex++;

		
/*

				// set new last block's last move and move sequence to old last block's
				bodyBlock.lastMove = snakeBody.back().lastMove;
				bodyBlock.moveSeq = snakeBody.back().moveSeq;

				glueToBack(snakeBody.back(), bodyBlock);

				snakeBody.push_back(bodyBlock);
	*/

				// new food position
				food.x = rand() % (SCREEN_W - food.w);
				food.y = rand() % (SCREEN_H - food.h) + SDL_GetTicks() % 10;
			} // end food intersection routine

			// screen edge collision
			if(player.x <= 0 || player.x+player.w >= SCREEN_W || player.y <= 0 || player.y+player.h >= SCREEN_H)
				playerIsDead = true;

			/*
			// collision with self
			for(int i = 1; i < snakeBody.size(); i++)
				if(SDL_HasIntersection(&player.rect, &snakeBody[i].rect))
					playerIsDead = true;

			// translate snake body blocks
			for(auto &b : snakeBody)
				updateObjPos(b);
			*/

			// render score
			int tmpScore = score;
			int tmpHighscore = highscore;
			for(int i=2;i>=0;i--)
			{
				SDL_RenderCopy(renderer, textures[tmpScore%10], NULL, &scoreRects[i]);
				SDL_RenderCopy(renderer, textures[tmpHighscore%10], NULL, &highscoreRects[i]);
				tmpScore/=10;
				tmpHighscore/=10;
			}

			// render player
			SDL_RenderCopy(renderer, textures[playerTexture], NULL, &player);

			// render snakeBody
			for(int i=0;i<sbIndex;i++) SDL_RenderCopy(renderer, textures[BODY], NULL, snakeBody+i);

			// render food 
			SDL_RenderCopy(renderer, textures[FOOD], NULL, &food);

			
		}
		else // player is dead
		{
			if(!playerDeathRoutineRan)
			{
				score = 0; // reset score

				// reset position
				player.x = SCREEN_W/2;
				player.y = SCREEN_H/2;

				// reset last move
				playerMove = UP;

				/*
				// reset snake body
				bodyBlock.lastMove = player.lastMove;
				bodyBlock.moveSeq.clear();
				glueToBack(player, bodyBlock);
				snakeBody.push_back(bodyBlock);
				*/

				// reset snake body
				sbIndex = 0;

				// new food position
				food.x = rand() % (SCREEN_W - food.w);
				food.y = rand() % (SCREEN_H - food.h) + SDL_GetTicks() % 10;

				playerDeathRoutineRan = true;
			}

			// player comes back
			if (SDL_TICKS_PASSED(SDL_GetTicks(), playerDeathTimeout))
				playerIsDead = playerDeathRoutineRan = false;
		}
		// render current textures
		renderPresent:

		SDL_RenderPresent(renderer);

		SDL_Delay(16);
	}

	// end game loop

	// clean up
	free(snakeBody);
	snakeBody = NULL;

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
