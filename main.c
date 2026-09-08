#include <stdio.h>
#include <SDL.h>
#include <time.h>
#include "I8080.h"

int loadROM(const char* filename, I8080* cpu) {
    FILE* inputFile = fopen(filename, "rb");

    if (inputFile == NULL) {
        return -1;
    }

    size_t i = 0;
    int byte = fgetc(inputFile);

    while (byte != EOF) {
        
        (cpu->memory)[i + 0x200] = byte;
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
    SDL_Window* window = SDL_CreateWindow(
        "CHIP8",                  // Window title
        SDL_WINDOWPOS_CENTERED,           // Initial x position
        SDL_WINDOWPOS_CENTERED,           // Initial y position
        1024,                              // Width, in pixels
        512,                              // Height, in pixels
        SDL_WINDOW_SHOWN                  // Flags
    );

    if (window == NULL) {
        printf("Window creation failed! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    if (renderer == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

    struct timespec req, rem;
    req.tv_sec = 0;
    req.tv_nsec = 16 * 1000 * 1000;

    int keepOpen = 1;

    while (keepOpen) {
        cycle(cpu);

        SDL_Event event;
        while(SDL_PollEvent(&event) > 0) {
            switch (event.type) {

                case SDL_QUIT:
                    keepOpen = 0;
                    break;
                // TODO: Key presses
                default:

            }
        }
    
        int RenderClear = SDL_RenderClear(renderer);

        uint32_t* bytes = NULL;
        int pitch = 0;

        SDL_LockTexture(texture, NULL, &bytes, &pitch);

        // for (size_t i = 0; i < 64*32; ++i) {
        //     bytes[i] = ((cpu->graphics)[i] == 1) ? 0xFFFFFFFF : 0x000000FF;
        // }

        // TODO: 

        SDL_UnlockTexture(texture);

        int RenderCopy = SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        nanosleep(&req, &rem);

    }

    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    free(cpu);

    return 0;
}