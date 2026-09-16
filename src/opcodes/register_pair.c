// REGISTER PAIR INSTRUCTIONS

#include "util.h"
#include "opcodes.h"

// Push Data Onto Stack
void PUSH(I8080* cpu) {
    uint8_t REG = ((cpu->opcode) >> 4) & 0x03;
    push_to_stack(cpu, REG * 2);

    increment_cycles(cpu, 11);
    increment_pc(cpu, 1);
}

// Pop Data Off Stack
void POP(I8080* cpu) {
    uint8_t REG = ((cpu->opcode) >> 4) & 0x03;
    pop_from_stack(cpu, REG * 2);

    increment_cycles(cpu, 10);
    increment_pc(cpu, 1);
}

// Double ADD
void DAD(I8080* cpu) {
    uint8_t REG = ((cpu->opcode) >> 4) & 0x03;
    uint16_t REG_v;
    uint32_t val;

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

    val = (uint32_t)HL + (uint32_t)REG_v;
    uint8_t H, L;
    H = (val >> 8);
    L = (val & 0x00FF);

    (cpu->registers)[4] = H;
    (cpu->registers)[5] = L;

    set_carry(cpu, val > 0xFFFF);

    increment_cycles(cpu, 10);
    increment_pc(cpu, 1);
}

// Increment register pair
void INX(I8080* cpu) {
    uint8_t REG = ((cpu->opcode) >> 4) & 0x03;
    uint16_t val;
    uint8_t H, L;

    switch (REG) {
        case 0:
            val = BC+1;
            H = (val >> 8);
            L = (val & 0x00FF);
            (cpu->registers)[0] = H;
            (cpu->registers)[1] = L;
            break;
        case 1:
            val = DE+1;
            H = (val >> 8);
            L = (val & 0x00FF);
            (cpu->registers)[2] = H;
            (cpu->registers)[3] = L;
            break;
        case 2:
            val = HL+1;
            H = (val >> 8);
            L = (val & 0x00FF);
            (cpu->registers)[4] = H;
            (cpu->registers)[5] = L;
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
    uint8_t REG = ((cpu->opcode) >> 4) & 0x03;
    uint16_t val;
    uint8_t H, L;

    switch (REG) {
        case 0:
            val = BC-1;
            H = (val >> 8);
            L = (val & 0x00FF);
            (cpu->registers)[0] = H;
            (cpu->registers)[1] = L;
            break;
        case 1:
            val = DE-1;
            H = (val >> 8);
            L = (val & 0x00FF);
            (cpu->registers)[2] = H;
            (cpu->registers)[3] = L;
            break;
        case 2:
            val = HL-1;
            H = (val >> 8);
            L = (val & 0x00FF);
            (cpu->registers)[4] = H;
            (cpu->registers)[5] = L;
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
void XTHL(I8080* cpu) { // TODO: update casting

    uint8_t temp1, temp2;
    temp1 = (cpu->registers)[5];
    temp2 = (cpu->registers)[4];

    (cpu->registers)[5] = (cpu->memory)[cpu->stack_pointer];
    (cpu->registers)[4] = (cpu->memory)[cpu->stack_pointer+1];

    (cpu->memory)[cpu->stack_pointer] = temp1;
    (cpu->memory)[cpu->stack_pointer+1] = temp2;

    increment_cycles(cpu, 18);
    increment_pc(cpu, 1);
}

// Load SP from H and L
void SPHL(I8080* cpu) {
    (cpu->stack_pointer) = HL;

    increment_cycles(cpu, 5);
    increment_pc(cpu, 1);
}
