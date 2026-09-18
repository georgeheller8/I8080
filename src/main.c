#include <stdio.h>
#include <time.h>

#include <SDL.h>
#include <SDL_timer.h>

#include "I8080.h"
#include "util.h"
#include "space.h"

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

static void generateInterrupt(I8080* cpu, uint8_t num) {
    // generates interrupt RST 1 when num = 1, RST 2 when num = 2

    if (!cpu->INTE) return;          
    cpu->INTE = 0;                  
    cpu->HALT = 0;                   
    push_to_stack(cpu, 8);
    cpu->program_counter = num * 8;    // RST 1 -> 0x0008, RST 2 -> 0x0010
    increment_cycles(cpu, 11);

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

    space* si = malloc(sizeof(space));
    init_space(si);

    I8080* cpu = malloc(sizeof(I8080));
    init_cpu(cpu, si);

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
        "Space Invaders",                  // Window title
        SDL_WINDOWPOS_CENTERED,           // Initial x position
        SDL_WINDOWPOS_CENTERED,           // Initial y position
        224,                              // Width, in pixels
        256,                              // Height, in pixels
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

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 244, 256);

    int frequency = SDL_GetPerformanceFrequency();

    int keepOpen = 1;

    int firstHalf = 1;

    cpu->memory[0x0005] = 0xC9;

    uint64_t frame_end;
    uint64_t next_int;
    uint8_t which;
    uint32_t* bytes = NULL;
    int pitch = 0;
    int start_time = 0;
    int current_time = 0;

    struct timespec req, rem;
    req.tv_sec = 0;
    req.tv_nsec = 1000000;

    const uint64_t frame_ticks = frequency / 60;
    uint64_t next_frame = SDL_GetPerformanceCounter() + frame_ticks;

    while (keepOpen) {

        frame_end = cpu->cycles + 33333;
        next_int = cpu->cycles + 16667;
        which = 1;

        while (cpu->cycles < frame_end) {
            if (cpu->cycles >= next_int) {
                generateInterrupt(cpu, which);
                which = 2;
                next_int += 16667;
            }
            cycle(cpu);
        }

        SDL_Event event;
        while(SDL_PollEvent(&event) > 0) {
            switch (event.type) {

                case SDL_QUIT:
                    keepOpen = 0;
                    break;
                // TODO: Add all necessary event polls/key presses
                default:
                    break;
            }
        }

        SDL_LockTexture(texture, NULL, &bytes, &pitch);

        // fill in upwards, then left to right. bytewise, up 32 times, right 224 times

        int memstart = 0x2400;
        int i = 0;

        for (int col = 0; col < 224; col++) {
            for (int row = 255; row >= 0; row-=8) {
                for (int bit = 0; bit < 8; bit++) {
                    bytes[((row-bit) * 224) + col] = ((cpu->memory[0x2400 + i]) & (1 << bit)) ? 0xFFFFFFFF: 0x000000FF;
                }
                i++;
            }
        }

        SDL_UnlockTexture(texture);

        int RenderCopy = SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        // use SDL timer to make sure that 16.67 milliseconds have passed

        while (SDL_GetPerformanceCounter() < next_frame) {
            SDL_Delay(1);
        }
        next_frame += frame_ticks;

    }

    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    
    free(cpu);
    free(si);

    return 0;
}