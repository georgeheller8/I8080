SRC = main.c I8080.c util.c space.c $(wildcard opcodes/*.c)

debug:
	gcc $(SRC) -I. -g -gdwarf-4 -o I8080 `sdl2-config --cflags --libs`
release:
	gcc $(SRC) -I. -o I8080 `sdl2-config --cflags --libs`
clean:
	rm -f I8080
	rm -rf I8080.dSYM
