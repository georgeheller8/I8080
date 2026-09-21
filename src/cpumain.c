#include <stdio.h>
#include <stdlib.h>

#include "I8080.h"
#include "util.h"

// Copies the ROM into memory starting at `origin`.
int loadROM(const char* filename, I8080* cpu, uint16_t origin) {
    FILE* inputFile = fopen(filename, "rb");

    if (inputFile == NULL) {
        return -1;
    }

    size_t i = origin;
    int byte = fgetc(inputFile);

    while (byte != EOF && i < sizeof(cpu->memory)) {
        
        (cpu->memory)[i] = byte;
        ++i;
        byte = fgetc(inputFile);

    }

    fclose(inputFile);
    return 0;
}

// Minimal CP/M BDOS emulation so the test ROMs can print to stdout
static void bdos(I8080* cpu) {
    switch (cpu->registers[1]) {
        case 2:                           
            putchar(cpu->registers[3]);
            break;
        case 9: {                         
            uint16_t a = DE;
            while (cpu->memory[a] != '$') putchar(cpu->memory[a++]);
            break;
        }
    }
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Error: no ROM file provided\n");
        return 1;
    }

    I8080* cpu = malloc(sizeof(I8080));
    init_cpu(cpu);

    const char* filename = argv[1];

    if (loadROM(filename, cpu, 0x0100) == -1) {
        printf("Error: file does not exist\n");
        free(cpu);
        return 1;
    }

    cpu->memory[0x0005] = 0xC9;

    while (1) {

        if (cpu->program_counter == 0x0005) bdos(cpu);
        if (cpu->program_counter == 0x0000) break;

        cycle(cpu);
    }

    printf("\n");
    fflush(stdout);

    free(cpu);

    return 0;
}
