// ROTATE ACCUMULATOR INSTRUCTIONS

#include "util.h"
#include "opcodes.h"

// Rotate accumulator left
void RLC(I8080* cpu) {
    set_carry(cpu, ACC >> 7);
    ACC = (ACC << 1);
    ACC |= CARRY;

    increment_cycles(cpu, 4);
    increment_pc(cpu, 1);
}

// Rotate accumulator right
void RRC(I8080* cpu) {
    set_carry(cpu, ACC & 1);
    ACC = (ACC >> 1);
    ACC |= (CARRY << 7);

    increment_cycles(cpu, 4);
    increment_pc(cpu, 1);
}

// Rotate accumulator left through carry
void RAL(I8080* cpu) {
    uint8_t CARRY_c = CARRY;
    set_carry(cpu, ACC >> 7);
    ACC = (ACC << 1);
    ACC |= CARRY_c;

    increment_cycles(cpu, 4);
    increment_pc(cpu, 1);
}

// Rotate accumulator right through carry
void RAR(I8080* cpu) {
    uint8_t CARRY_c = CARRY;
    set_carry(cpu, ACC & 0x01);
    ACC = (ACC >> 1);
    ACC |= (CARRY_c << 7);

    increment_cycles(cpu, 4);
    increment_pc(cpu, 1);
}
