#ifndef I8080_H
#define I8080_H

#include <stdint.h>

typedef struct {

    // Registers

    uint8_t wz[2]; // WZ -> 01
    uint8_t registers[8]; // BCDEHLAF -> 01234567
    uint16_t program_counter;
    uint16_t stack_pointer;
    uint8_t opcode;
    uint8_t INTE;
    uint8_t HALT;

    uint8_t memory[65536];

    uint64_t cycles;


} I8080;

// Public interface. Everything else is internal to the emulator core: the
// stack/counter and flag helpers are declared in util.h, the instruction set
// in opcodes/opcodes.h.

void init_cpu(I8080* cpu);
void cycle(I8080* cpu);

#endif // I8080_H
