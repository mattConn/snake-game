snake-game: *.cpp
	em++ *.cpp -o snake.html -g -lm --bind -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' --preload-file assets/ --use-preload-plugins 

check:
	./snake-game

clean:
	rm snake-game
