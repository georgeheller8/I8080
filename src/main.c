#include <stdio.h>
#include <SDL.h>
#include <time.h>
#include "I8080.h"

static void bdos(I8080* cpu) {
    switch (cpu->registers[1]) {          // C
        case 2:                           // print char in E
            putchar(cpu->registers[3]);
            break;
        case 9: {                         // print '$'-terminated string at DE
            uint16_t a = ((uint16_t)cpu->registers[2] << 8) | cpu->registers[3];
            while (cpu->memory[a] != '$') putchar(cpu->memory[a++]);
            break;
        }
    }
}

int loadROM(const char* filename, I8080* cpu) {
    FILE* inputFile = fopen(filename, "rb");

    if (inputFile == NULL) {
        return -1;
    }

    size_t i = 0;
    int byte = fgetc(inputFile);

    while (byte != EOF) {
        
        (cpu->memory)[i + 0x0100] = byte;
        ++i;
        byte = fgetc(inputFile);

    }

    fclose(inputFile);
    return 0;
}

int main(int argc, char* argv[]) {
    // 1. Initialize SDL Video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    I8080* cpu = malloc(sizeof(I8080));
    init_cpu(cpu);

    if (argc < 2) {
        printf("Error: no ROM file provided\n");
        return 1;
    }

    const char* filename = argv[1];

    int romStatus = loadROM(filename, cpu);

    if (romStatus == -1) {
        printf("Error: file does not exist\n");
        return 1;
    }


    // 2. Create a Window
    // SDL_Window* window = SDL_CreateWindow(
    //     "CHIP8",                  // Window title
    //     SDL_WINDOWPOS_CENTERED,           // Initial x position
    //     SDL_WINDOWPOS_CENTERED,           // Initial y position
    //     1024,                              // Width, in pixels
    //     512,                              // Height, in pixels
    //     SDL_WINDOW_SHOWN                  // Flags
    // );

    // if (window == NULL) {
    //     printf("Window creation failed! SDL_Error: %s\n", SDL_GetError());
    //     SDL_Quit();
    //     return 1;
    // }

    // SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // if (renderer == NULL) {
    //     printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    //     SDL_Quit();
    //     return 1;
    // }

    // SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

    struct timespec req, rem;
    req.tv_sec = 0;
    req.tv_nsec = 16 * 1000 * 1000;

    int keepOpen = 1;

    cpu->memory[0x0005] = 0xC9;

    while (keepOpen) {

        if (cpu->program_counter == 0x0005) bdos(cpu);
        if (cpu->program_counter == 0x0000) break;

        cycle(cpu);
    }

    // SDL_DestroyWindow(window);
    // SDL_DestroyTexture(texture);
    // SDL_DestroyRenderer(renderer);
    // SDL_Quit();
    free(cpu);

    return 0;
}