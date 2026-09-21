CORE = src/I8080.c src/util.c $(wildcard src/opcodes/*.c)
CFLAGS = -Iinclude

default: cpu space

cpu:
	gcc src/cpumain.c $(CORE) $(CFLAGS) -o cpu
space:
	gcc src/spacemain.c src/space.c $(CORE) $(CFLAGS) -DSPACE_INVADERS -o space `sdl2-config --cflags --libs`
clean:
	rm -f cpu space

.PHONY: cpu space clean
