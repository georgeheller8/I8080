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
        (cpu->memory)[HL] += 1;
        result = (cpu->memory)[HL];
        increment_cycles(cpu, 5);
    }
    else if (reg == 7) {
        (cpu->registers)[6] += 1;
        result = (cpu->registers)[6];
    }

    update_aux_carry(cpu, result-1, 1, 0);
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
        (cpu->memory)[HL] -= 1;
        result = (cpu->memory)[HL];
        increment_cycles(cpu, 5);
    }
    else if (reg == 7) {
        (cpu->registers)[6] -= 1;
        result =  (cpu->registers)[6];
    }

    update_aux_carry(cpu, result+1, 1, 1);
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
void DAA(I8080* cpu) { // TODO: fix

    uint8_t correction = 0;
    uint8_t carry = CARRY;

    if ((ACC & 0x0F) > 9 || AUX_CARRY) correction |= 0x06;
    if (ACC > 0x99 || CARRY) { correction |= 0x60; carry = 1; }
    update_aux_carry(cpu, ACC, correction, 0);
    ACC += correction;
    set_carry(cpu, carry);

    update_zero(cpu, ACC);
    update_sign(cpu, ACC);
    update_parity(cpu, ACC);
    increment_pc(cpu, 1);
    increment_cycles(cpu, 4);

}
