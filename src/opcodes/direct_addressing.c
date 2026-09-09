// DIRECT ADDRESSING INSTRUCTIONS

#include "util.h"
#include "opcodes.h"

// Store Accumulator Direct
void STA(I8080* cpu) {
    uint16_t LSB = (cpu->memory)[cpu->program_counter + 1];
    uint16_t MSB = (cpu->memory)[cpu->program_counter + 2];

    (cpu->memory)[(MSB << 8) | LSB] = ACC;

    increment_cycles(cpu, 13);
    increment_pc(cpu, 3);
}

// Load Accumulator Direct
void LDA(I8080* cpu) {
    uint16_t LSB = (cpu->memory)[cpu->program_counter + 1];
    uint16_t MSB = (cpu->memory)[cpu->program_counter + 2];

    ACC = (cpu->memory)[(MSB << 8) | LSB];

    increment_cycles(cpu, 13);
    increment_pc(cpu, 3);
}

// Store H and L direct
void SHLD(I8080* cpu) { // TODO: Update casting
    uint16_t LSB = (cpu->memory)[cpu->program_counter + 1];
    uint16_t MSB = (cpu->memory)[cpu->program_counter + 2];

    (cpu->memory)[(MSB << 8) | LSB] = (cpu->registers)[5];
    (cpu->memory)[((MSB << 8) | LSB) + 1] = (cpu->registers)[4];

    increment_cycles(cpu, 16);
    increment_pc(cpu, 3);
}

// Load H and L Direct
void LHLD(I8080* cpu) { // TODO: Update casting
    uint16_t LSB = (cpu->memory)[cpu->program_counter + 1];
    uint16_t MSB = (cpu->memory)[cpu->program_counter + 2];

    (cpu->registers)[5] = (cpu->memory)[(MSB << 8) | LSB];
    (cpu->registers)[4] = (cpu->memory)[((MSB << 8) | LSB) + 1];

    increment_cycles(cpu, 16);
    increment_pc(cpu, 3);
}
