# Intel 8080 Emulator

Cycle-counting Intel 8080 emulator, with additional hardware emulation to support Space Invaders.

## Installation/Dependencies

This project uses SDL2. The Makefile requires that `sdl2-config` be located on your PATH. To get it:

- **macOS (Homebrew):** `brew install sdl2`
- **Debian/Ubuntu:** `sudo apt install libsdl2-dev`
- **Fedora:** `sudo dnf install SDL2-devel`
- **Arch:** `sudo pacman -S sdl2`

To check that it's set up, run `sdl2-config --cflags --libs`. It should print compiler and linker flags, not "command not found".


## Building/Running

There are two separate build targets for this project: a raw CPU emulator and the same emulator with additional hardware support for space invaders. To build them, in the project root run:

- CPU-only: `make cpu`
- Space Invaders: `make space`

The generic CPU emulator accepts test ROMs and writes to stdout. To run a test rom, in the project root run:

`./cpu <path/to/rom>`

The Space Invaders emulator loads the space invaders ROM by default. To run this emulator, in the project root run:

`./space`

Note that this project uses relative pathing, so all executables must be run from the project root.

## Playing Space Invaders

Space invaders in this project is designed for singeplayer only. While the game itself says that it is a two-player game, the ROM does not actually have support for two players.

Controls:
- Add Credit: c
- Start Game: o (t also works when two credits are inserted)
- Move Left: a
- Move Right: r
- Shoot: w

## Credits

Much of the SDL setup came from [this](https://www.youtube.com/watch?v=YHkBgR6yvbY&t=3421s) YouTube video on CHIP-8 emulators, and guidance on Space Invaders hardware setup came from [here](https://pranayga.github.io/8080-emulator/).


