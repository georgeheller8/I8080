// DATA TRANSFER INSTRUCTIONS

#include "util.h"
#include "opcodes.h"

// Move
void MOV(I8080* cpu) {

    uint8_t SRC = (cpu->opcode) & 0x07;
    uint8_t DEST = ((cpu->opcode) >> 3) & 0x07;

    uint8_t SRC_v;

    if (SRC <= 5) {
        SRC_v = (cpu->registers)[SRC];
    }
    else if (SRC == 6) {
        SRC_v = (cpu->memory)[HL];
    }
    else if (SRC == 7) {
        SRC_v = (cpu->registers)[6];
    }

    if (DEST <= 5) {
        (cpu->registers)[DEST] = SRC_v;
    }
    else if (DEST == 6) {
        (cpu->memory)[HL] = SRC_v;
    }
    else if (DEST == 7) {
        (cpu->registers)[6] = SRC_v;
    }

    increment_pc(cpu, 1);

    if (SRC != 5 && DEST != 5) {
        increment_cycles(cpu, 5);
    }
    else {
        increment_cycles(cpu, 7);
    }

}

// Store accumulator
void STAX(I8080* cpu) {

    (cpu->memory)[(((cpu->opcode) & 0x10) >> 4) ? DE : BC] = ACC;

    increment_pc(cpu, 1);
    increment_cycles(cpu, 7);

}

// Load accumulator
void LDAX(I8080* cpu) {

    ACC = (cpu->memory)[(((cpu->opcode) & 0x10) >> 4) ? DE : BC];

    increment_pc(cpu, 1);
    increment_cycles(cpu, 7);

}
