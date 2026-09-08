debug:
	gcc main.c I8080.c -g -o I8080 `sdl2-config --cflags --libs`
release:
	gcc main.c I8080.c -o I8080 `sdl2-config --cflags --libs`
clean:
	rm I8080
	rm -rf I8080.dSYM