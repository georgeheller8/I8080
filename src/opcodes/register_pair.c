// REGISTER PAIR INSTRUCTIONS

#include "util.h"
#include "opcodes.h"

// Push Data Onto Stack
void PUSH(I8080* cpu) {
    uint8_t REG = (cpu->opcode) >> 4;
    push_to_stack(cpu, REG * 2);

    increment_cycles(cpu, 11);
    increment_pc(cpu, 1);
}

// Pop Data Off Stack
void POP(I8080* cpu) {
    uint8_t REG = (cpu->opcode) >> 4;
    pop_from_stack(cpu, REG * 2);

    increment_cycles(cpu, 10);
    increment_pc(cpu, 1);
}

// Double ADD
void DAD(I8080* cpu) {
    uint8_t REG = (cpu->opcode) >> 4;
    uint16_t REG_v;
    uint16_t val;

    switch (REG) {
        case 0:
            REG_v = BC;
            break;
        case 1:
            REG_v = DE;
            break;
        case 2:
            REG_v = HL;
            break;
        case 3:
            REG_v = (cpu->stack_pointer);
            break;
    }

    val = HL + REG_v;
    uint8_t H, L;
    H = (val >> 8);
    L = (val & 0x00FF);

    (cpu->registers)[4] = H;
    (cpu->registers)[5] = L;

    update_carry(cpu, H, L, 0);

    increment_cycles(cpu, 10);
    increment_pc(cpu, 1);
}

// Increment register pair
void INX(I8080* cpu) {
    uint8_t REG = (cpu->opcode) >> 4;
    uint16_t REG_v;
    uint16_t val;
    uint8_t H, L;

    switch (REG) {
        case 0:
            REG_v = BC+1;
            H = (val >> 8);
            L = (val & 0x00FF);
            (cpu->registers)[0] = H;
            (cpu->registers)[1] = L;
            break;
        case 1:
            REG_v = DE+1;
            H = (val >> 8);
            L = (val & 0x00FF);
            (cpu->registers)[0] = H;
            (cpu->registers)[1] = L;
            break;
        case 2:
            REG_v = HL+1;
            H = (val >> 8);
            L = (val & 0x00FF);
            (cpu->registers)[0] = H;
            (cpu->registers)[1] = L;
            break;
        case 3:
            (cpu->stack_pointer) += 1;
            break;
    }

    increment_cycles(cpu, 5);
    increment_pc(cpu, 1);
}

// Decrement register pair
void DCX(I8080* cpu) {
    uint8_t REG = (cpu->opcode) >> 4;
    uint16_t REG_v;
    uint16_t val;
    uint8_t H, L;

    switch (REG) {
        case 0:
            REG_v = BC-1;
            H = (val >> 8);
            L = (val & 0x00FF);
            (cpu->registers)[0] = H;
            (cpu->registers)[1] = L;
            break;
        case 1:
            REG_v = DE-1;
            H = (val >> 8);
            L = (val & 0x00FF);
            (cpu->registers)[0] = H;
            (cpu->registers)[1] = L;
            break;
        case 2:
            REG_v = HL-1;
            H = (val >> 8);
            L = (val & 0x00FF);
            (cpu->registers)[0] = H;
            (cpu->registers)[1] = L;
            break;
        case 3:
            (cpu->stack_pointer) -= 1;
            break;
    }

    increment_cycles(cpu, 5);
    increment_pc(cpu, 1);
}

// Exchange registers
void XCHG(I8080* cpu) {
    uint16_t temp = HL;

    (cpu->registers)[4] = (cpu->registers)[2];
    (cpu->registers)[5] = (cpu->registers)[3];

    (cpu->registers)[2] = (temp >> 8);
    (cpu->registers)[3] = (temp & 0x00FF);

    increment_cycles(cpu, 4);
    increment_pc(cpu, 1);
}


// Exchange stack
void XTHL(I8080* cpu) {
    (cpu->registers)[5] = (cpu->memory)[cpu->stack_pointer];
    (cpu->registers)[4] = (cpu->memory)[cpu->stack_pointer+1];

    increment_cycles(cpu, 18);
    increment_pc(cpu, 1);
}

// Load SP from H and L
void SPHL(I8080* cpu) {
    (cpu->stack_pointer) = HL;

    increment_cycles(cpu, 5);
    increment_pc(cpu, 1);
}
