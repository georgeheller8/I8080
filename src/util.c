// Helper functions shared by the opcode implementations in opcodes/.
// Declared in util.h.

#include "util.h"

void push_to_stack(I8080* cpu, uint8_t reg) {

    // Register options: 0 is BC pair, 2, is DE pair, 4 is HL pair, 6 is program counter

    uint8_t MSB, LSB;

    if (reg == 8) {
        MSB = ((cpu->program_counter) >> 8);
        LSB = ((cpu->program_counter) & 0x00FF);
    }
    else {
        MSB = (cpu->registers)[reg];
        LSB = (cpu->registers)[reg+1];
    }

    (cpu->memory)[--(cpu->stack_pointer)] = MSB;
    (cpu->memory)[--(cpu->stack_pointer)] = LSB;

}

void pop_from_stack(I8080* cpu, uint8_t reg) {

    // LSB is at address of stack pointer, MSB is at address of stack pointer + 1

    if (reg == 8) {
        (cpu->program_counter) = (cpu->memory)[(cpu->stack_pointer)++];
        (cpu->program_counter) |= ((uint16_t)((cpu->memory)[(cpu->stack_pointer)++]) << 8);
    }
    else {
        (cpu->registers)[reg+1] = (cpu->memory)[(cpu->stack_pointer)++];
        (cpu->registers)[reg] = (cpu->memory)[(cpu->stack_pointer)++];
    }

}

void increment_cycles(I8080* cpu, uint8_t num) {
    cpu->cycles += num;
}

void increment_pc(I8080* cpu, uint8_t num) {
    cpu->program_counter += num;
}

// flag checkers

void set_zero(I8080* cpu, uint8_t num) {
    (cpu->registers)[7] &= 0xBF;
    (cpu->registers)[7] |= (num << 6);
}

void set_sign(I8080* cpu, uint8_t num) {
    (cpu->registers)[7] &= 0x7F;
    (cpu->registers)[7] |= (num << 7);
}

void set_carry(I8080* cpu, uint8_t num) {
    (cpu->registers)[7] &= 0xFE;
    (cpu->registers)[7] |= num;
}

void set_aux_carry(I8080* cpu, uint8_t num) {
    (cpu->registers)[7] &= 0xEF;
    (cpu->registers)[7] |= (num << 4);
}

void set_parity(I8080* cpu, uint8_t num) {
    (cpu->registers)[7] &= 0xFB;
    (cpu->registers)[7] |= (num << 2);
}

void update_zero(I8080* cpu, uint8_t result) {
    (cpu->registers)[7] &= 0xBF;
    (cpu->registers)[7] |= ((result == 0) << 6);
}

void update_sign(I8080* cpu, uint8_t result) {
    (cpu->registers)[7] &= 0x7F;
    (cpu->registers)[7] |= (result & 0x80);
}

void update_carry(I8080* cpu, uint8_t A, uint8_t B, uint8_t OP) {
    // OP = 0 for addition, 1 for subtraction
    // In subtraction cases, B is being subtracted from A
    uint16_t check;
    uint8_t CARRY_v = CARRY;
    check = A + B;

    (cpu->registers)[7] &= 0xFE;

    if (OP == 0) {
        (cpu->registers)[7] |= (check >= 256);
    }
    else if (OP == 1) {
        (cpu->registers)[7] |= (A >= B);
    }
    else if (OP == 2) {
        check += CARRY_v;
        (cpu->registers)[7] |= (check >= 256);
    }

}

void update_aux_carry(I8080* cpu, uint8_t A, uint8_t B, uint8_t OP) {
    // OP = 0 for addition, 1 for subtraction

    uint8_t Al = A & 0x0F;
    uint8_t Bl = B & 0x0F;
    uint8_t CARRY_v = CARRY;

    uint8_t check = Al + Bl;

    (cpu->registers)[7] &= 0xEF;

    if (OP == 0) {
        (cpu->registers)[7] |= ((check >= 16) << 4);
    }
    else if (OP == 1) {
        (cpu->registers)[7] |= ((Al >= Bl) << 4);
    }
    else if (OP == 2) {
        check += CARRY_v;
        (cpu->registers)[7] |= ((check >= 16) << 4);
    }

}

void update_parity(I8080* cpu, uint8_t result) {
    (cpu->registers)[7] &= 0xFB;
    (cpu->registers)[7] |= (((__builtin_popcount(result) % 2) == 0) << 2);
}
