// CARRY BIT INSTRUCTIONS

#include "util.h"
#include "opcodes.h"

// Set carry
void STC(I8080* cpu) {
    (cpu->registers)[7] |= 0x01;
    increment_pc(cpu, 1);
    increment_cycles(cpu, 4);
}

// Complement carry
void CMC(I8080* cpu) {
    (cpu->registers)[7] ^= 0x01;
    increment_pc(cpu, 1);
    increment_cycles(cpu, 4);
}
