// JUMP INSTRUCTIONS

#include "util.h"
#include "opcodes.h"

// Load Program Counter
void PCHL(I8080* cpu) {
    (cpu->program_counter) = HL;
    increment_cycles(cpu, 5);
}

// Jump
void JMP(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    cpu->program_counter = address;
    increment_cycles(cpu, 10);
}

// Jump if Carry
void JC(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    if (CARRY) cpu->program_counter = address;
    else increment_pc(cpu, 3);
    increment_cycles(cpu, 10);
}

// Jump if No Carry
void JNC(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    if (!CARRY) cpu->program_counter = address;
    else increment_pc(cpu, 3);
    increment_cycles(cpu, 10);
}

// Jump if Zero
void JZ(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    if (ZERO) cpu->program_counter = address;
    else increment_pc(cpu, 3);
    increment_cycles(cpu, 10);
}

// Jump if Not Zero
void JNZ(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    if (!ZERO) cpu->program_counter = address;
    else increment_pc(cpu, 3);
    increment_cycles(cpu, 10);
}

// Jump if Minus
void JM(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    if (SIGN) cpu->program_counter = address;
    else increment_pc(cpu, 3);
    increment_cycles(cpu, 10);
}

// Jump if Positive
void JP(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    if (!SIGN) cpu->program_counter = address;
    else increment_pc(cpu, 3);
    increment_cycles(cpu, 10);
}

// Jump if Parity Even
void JPE(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    if (PARITY) cpu->program_counter = address;
    else increment_pc(cpu, 3);
    increment_cycles(cpu, 10);
}

// Jump if Parity Odd
void JPO(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    if (!PARITY) cpu->program_counter = address;
    else increment_pc(cpu, 3);
    increment_cycles(cpu, 10);
}
