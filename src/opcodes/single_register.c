// SINGLE REGISTER INSTRUCTIONS

#include "util.h"
#include "opcodes.h"

// Increment register
void INR(I8080* cpu) {

    uint8_t reg = (cpu->opcode) >> 3;
    uint8_t result;

    if (reg < 6) {
        (cpu->registers)[reg] += 1;
        result = (cpu->registers)[reg];
    }
    else if (reg == 6) {
        (cpu->memory)[((cpu->registers)[5] << 8) | (cpu->registers)[6]] += 1;
        result = (cpu->memory)[((cpu->registers)[5] << 8) | (cpu->registers)[6]];
    }
    else if (reg == 7) {
        (cpu->registers)[6] += 1;
        result = (cpu->registers)[6];
    }

    update_sign(cpu, result);
    update_zero(cpu, result);
    update_parity(cpu, result);
    increment_pc(cpu, 1);
    increment_cycles(cpu, 5);

}

// Decrement register
void DCR(I8080* cpu) {

    uint8_t reg = (cpu->opcode) >> 3;
    uint8_t result;

    if (reg < 6) {
        (cpu->registers)[reg] -= 1;
        result = (cpu->registers)[reg];
    }
    else if (reg == 6) {
        (cpu->memory)[((cpu->registers)[5] << 8) | (cpu->registers)[6]] -= 1;
        result = (cpu->memory)[((cpu->registers)[5] << 8) | (cpu->registers)[6]];
    }
    else if (reg == 7) {
        (cpu->registers)[6] -= 1;
        result =  (cpu->registers)[6];
    }

    update_sign(cpu, result);
    update_zero(cpu, result);
    update_parity(cpu, result);
    increment_pc(cpu, 1);
    increment_cycles(cpu, 5);

}

// Complement accumulator
void CMA(I8080* cpu) {

    (cpu->registers)[6] ^= 0xFF;
    increment_pc(cpu, 1);
    increment_cycles(cpu, 4);
}


// Decimal adjust accumulator
void DAA(I8080* cpu) {

    uint8_t LSB = ACC & 0x0F;
    uint8_t MSB = ACC >> 4;

    if (LSB > 9 || AUX_CARRY) {
        update_aux_carry(cpu, ACC, 6, 0);
        ACC += 6;
        MSB = (ACC >> 4);
    }
    if (MSB > 9 || CARRY) {
        (cpu->registers)[7] |= (ACC + 0x60 > 255);
        ACC += 0x60;
    }

    update_zero(cpu, ACC);
    update_sign(cpu, ACC);
    update_parity(cpu, ACC);
    increment_pc(cpu, 1);
    increment_cycles(cpu, 4);

}
