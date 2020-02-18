snake-game: *.c
	gcc -I/usr/include/SDL2 $^ -o $@ -lSDL2 -lSDL2_image


check:
	./snake-game

clean:
	rm snake-game
