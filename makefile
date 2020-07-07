snake-game: *.cpp
	clang++ -std=c++11 -I /usr/include/SDL2/ -l SDL2 -l SDL2_image $^ -o $@

snake.html: *.cpp
	em++ $^ -o $@ -g -lm --bind -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' --preload-file assets/ --use-preload-plugins --shell-file template.html

check:
	./snake-game

clean:
	rm snake-game
