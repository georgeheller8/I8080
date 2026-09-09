// REGISTER OR MEMORY TO ACCUMULATOR INSTRUCTIONS

#include "util.h"
#include "opcodes.h"

// ADD Register or Memory to Accumulator
void ADD(I8080* cpu) {

    uint8_t SRC = (cpu->opcode) & 0x07;
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

    update_carry(cpu, SRC_v, ACC, 0);
    update_aux_carry(cpu, SRC_v, ACC, 0);

    ACC += SRC_v;

    update_sign(cpu, ACC);
    update_zero(cpu, ACC);
    update_parity(cpu, ACC);

    if (SRC != 6) {
        increment_cycles(cpu, 4);
    }
    else {
        increment_cycles(cpu, 7);
    }

    increment_pc(cpu, 1);

}

// ADD Register or Memory to Accumulator with Carry
void ADC(I8080* cpu) {

    uint8_t SRC = (cpu->opcode) & 0x07;
    uint8_t SRC_v;
    uint8_t CARRY_v = CARRY;

    if (SRC <= 5) {
        SRC_v = (cpu->registers)[SRC];
    }
    else if (SRC == 6) {
        SRC_v = (cpu->memory)[HL];
    }
    else if (SRC == 7) {
        SRC_v = (cpu->registers)[6];
    }

    update_carry(cpu, SRC_v, ACC, 2);
    update_aux_carry(cpu, SRC_v, ACC, 2);

    ACC += (SRC_v + CARRY_v);

    update_sign(cpu, ACC);
    update_zero(cpu, ACC);
    update_parity(cpu, ACC);

    if (SRC != 6) {
        increment_cycles(cpu, 4);
    }
    else {
        increment_cycles(cpu, 7);
    }

    increment_pc(cpu, 1);
}

// Subtrack Register or Memory from Accumulator
void SUB(I8080* cpu) {
    uint8_t SRC = (cpu->opcode) & 0x07;
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

    update_carry(cpu, ACC, SRC_v, 1);
    update_aux_carry(cpu, ACC, SRC_v, 1);

    ACC -= SRC_v;

    update_sign(cpu, ACC);
    update_zero(cpu, ACC);
    update_parity(cpu, ACC);

    if (SRC != 6) {
        increment_cycles(cpu, 4);
    }
    else {
        increment_cycles(cpu, 7);
    }

    increment_pc(cpu, 1);
}

// subtract with borrow
void SBB(I8080* cpu) {
    uint8_t SRC = (cpu->opcode) & 0x07;
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

    uint8_t CARRY_v = CARRY;
    uint16_t res = (uint16_t)SRC_v + CARRY_v;

    set_carry(cpu, ACC < res);
    set_aux_carry(cpu, (ACC & 0x0F) >= ((SRC_v & 0x0F) + CARRY_v));

    ACC -= (SRC_v + CARRY_v);

    update_sign(cpu, ACC);
    update_zero(cpu, ACC);
    update_parity(cpu, ACC);

    if (SRC != 6) {
        increment_cycles(cpu, 4);
    }
    else {
        increment_cycles(cpu, 7);
    }

    increment_pc(cpu, 1);
}

// Logical AND acc with memory or register
void ANA(I8080* cpu) { // need to fix clearing aux carry?
    uint8_t SRC = (cpu->opcode) & 0x07;
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

    ACC &= SRC_v;
    (cpu->registers)[7] &= 0xFE;

    update_sign(cpu, ACC);
    update_zero(cpu, ACC);
    update_parity(cpu, ACC);

    if (SRC != 6) {
        increment_cycles(cpu, 4);
    }
    else {
        increment_cycles(cpu, 7);
    }

    increment_pc(cpu, 1);

}

// Logical XOR acc with memory or register
void XRA(I8080* cpu) {
     uint8_t SRC = (cpu->opcode) & 0x07;
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

    ACC ^= SRC_v;
    (cpu->registers)[7] &= 0xFE;
    (cpu->registers)[7] &= 0xEF; // is this right?

    update_sign(cpu, ACC);
    update_zero(cpu, ACC);
    update_parity(cpu, ACC);

    if (SRC != 6) {
        increment_cycles(cpu, 4);
    }
    else {
        increment_cycles(cpu, 7);
    }

    increment_pc(cpu, 1);
}

// Logical OR with memory or register
void ORA(I8080* cpu) {
    uint8_t SRC = (cpu->opcode) & 0x07;
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

    ACC |= SRC_v;
    (cpu->registers)[7] &= 0xFE;
    (cpu->registers)[7] &= 0xEF; // is this right?

    update_sign(cpu, ACC);
    update_zero(cpu, ACC);
    update_parity(cpu, ACC);

    if (SRC != 6) {
        increment_cycles(cpu, 4);
    }
    else {
        increment_cycles(cpu, 7);
    }

    increment_pc(cpu, 1);
}

// Compare Register or Memory with Accumulator
void CMP(I8080* cpu) {
    uint8_t SRC = (cpu->opcode) & 0x07;
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

    uint8_t ACC_v;
    ACC_v = ACC;

    if (ACC_v > SRC_v) {
        (cpu->registers)[7] &= 0xFE;
        (cpu->registers)[7] &= ~(1 << 6);
    }
    else if (ACC_v == SRC_v) {
        (cpu->registers)[7] |= (1 << 6);
    }
    else if (ACC_v < SRC_v) {
        (cpu->registers)[7] |= 0x01;
        (cpu->registers)[7] &= ~(1 << 6);
    }

    update_sign(cpu, ACC_v - SRC_v);
    update_zero(cpu, ACC_v - SRC_v);
    update_parity(cpu, ACC_v - SRC_v);

    if (SRC != 6) {
        increment_cycles(cpu, 4);
    }
    else {
        increment_cycles(cpu, 7);
    }

    increment_pc(cpu, 1);

}
