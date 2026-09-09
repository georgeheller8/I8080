// IMMEDIATE INSTRUCTIONS

#include "util.h"
#include "opcodes.h"

// Load immediate data
void LXI(I8080* cpu) {
    uint8_t reg = (cpu->opcode >> 4) & 0x03;
    uint8_t MSB = (cpu->memory)[cpu->program_counter + 2];
    uint8_t LSB = (cpu->memory)[cpu->program_counter + 1];

    switch (reg) {
        case 0: // BC
            (cpu->registers)[0] = MSB;
            (cpu->registers)[1] = LSB;
            break;
        case 1: // DE
            (cpu->registers)[2] = MSB;
            (cpu->registers)[3] = LSB;
            break;
        case 2: // HL
            (cpu->registers)[4] = MSB;
            (cpu->registers)[5] = LSB;
            break;
        case 3: // SP
            cpu->stack_pointer = ((uint16_t)MSB << 8) | LSB;
            break;
    }

    increment_pc(cpu, 3);
    increment_cycles(cpu, 10);

}

// Move Immediate Data
void MVI(I8080* cpu) {
    uint8_t reg = (cpu->opcode >> 3);
    uint8_t data = (cpu->memory)[cpu->program_counter + 1];

    if (reg < 6) {
        (cpu->registers)[reg] = data;
    }
    else if (reg == 6) {
        (cpu->memory)[HL] = data;
    }
    else if (reg == 7) {
        (cpu->registers)[6] = data;
    }

    if (reg != 6) {
        increment_cycles(cpu, 7);
    }
    else {
        increment_cycles(cpu, 10);
    }

    increment_pc(cpu, 2);
}

// Add Immediate to Accumulator
void ADI(I8080* cpu) {
    uint8_t data = (cpu->memory)[cpu->program_counter + 1];

    update_carry(cpu, data, ACC, 0);
    update_aux_carry(cpu, data, ACC, 0);

    ACC += data;

    update_zero(cpu, ACC);
    update_sign(cpu, ACC);
    update_parity(cpu, ACC);

    increment_cycles(cpu, 7);
    increment_pc(cpu, 2);
}

// Add Immediate with Carry
void ACI(I8080* cpu) {
    uint8_t data = (cpu->memory)[cpu->program_counter + 1];
    uint8_t CARRY_v = CARRY;

    set_carry(cpu, (uint16_t)(data + ACC + CARRY) > 255);
    set_aux_carry(cpu, (data & 0x0F) + (ACC & 0x0F) + CARRY > 15);

    ACC += (data + CARRY_v);

    update_zero(cpu, ACC);
    update_parity(cpu, ACC);
    update_sign(cpu, ACC);

    increment_cycles(cpu, 7);
    increment_pc(cpu, 2);

}

// Subtract Immediate From Accumulator
void SUI(I8080* cpu) {
    uint8_t data = (cpu->memory)[cpu->program_counter + 1];

    update_carry(cpu, ACC, data, 1);
    update_aux_carry(cpu, ACC, data, 1);

    ACC -= data;

    update_zero(cpu, ACC);
    update_parity(cpu, ACC);
    update_sign(cpu, ACC);

    increment_cycles(cpu, 7);
    increment_pc(cpu, 2);
}

// Subtract Immediate from Accumulator with Borrow
void SBI(I8080* cpu) {
    uint8_t data = (cpu->memory)[cpu->program_counter + 1];
    uint8_t CARRY_v = CARRY;
    uint16_t res = CARRY + data;

    set_carry(cpu, ACC < res);
    set_aux_carry(cpu, (ACC & 0x0F) >= ((res & 0x0F) + CARRY_v));

    ACC -= CARRY_v + data;

    update_zero(cpu, ACC);
    update_parity(cpu, ACC);
    update_sign(cpu, ACC);

    increment_cycles(cpu, 7);
    increment_pc(cpu, 2);
}

// And Immediate with Accumulator
void ANI(I8080* cpu) {
    uint8_t data = (cpu->memory)[cpu->program_counter + 1];

    ACC &= data;

    set_carry(cpu, 0);
    set_aux_carry(cpu, 0);
    update_zero(cpu, ACC);
    update_sign(cpu, ACC);
    update_parity(cpu, ACC);

    increment_cycles(cpu, 7);
    increment_pc(cpu, 2);
}

// XOR Immediate with Accumulator
void XRI(I8080* cpu) {
    uint8_t data = (cpu->memory)[cpu->program_counter + 1];

    ACC ^= data;

    set_carry(cpu, 0);
    set_aux_carry(cpu, 0);
    update_zero(cpu, ACC);
    update_sign(cpu, ACC);
    update_parity(cpu, ACC);

    increment_cycles(cpu, 7);
    increment_pc(cpu, 2);
}

// Or Immediate with Accumulator
void ORI(I8080* cpu) {
    uint8_t data = (cpu->memory)[cpu->program_counter + 1];

    ACC |= data;

    set_carry(cpu, 0);
    set_aux_carry(cpu, 0);
    update_zero(cpu, ACC);
    update_sign(cpu, ACC);
    update_parity(cpu, ACC);

    increment_cycles(cpu, 7);
    increment_pc(cpu, 2);
}

// Compare Immediate with Accumulator
void CPI(I8080* cpu) {
    uint8_t data = (cpu->memory)[cpu->program_counter + 1];

    if (ACC > data) {
        (cpu->registers)[7] &= 0xFE;
        (cpu->registers)[7] &= ~(1 << 6);
    }
    else if (ACC == data) {
        (cpu->registers)[7] |= (1 << 6);
    }
    else if (ACC < data) {
        (cpu->registers)[7] |= 0x01;
        (cpu->registers)[7] &= ~(1 << 6);
    }

    update_sign(cpu, ACC-data);
    update_zero(cpu, ACC-data);
    update_parity(cpu, ACC-data);

    increment_cycles(cpu, 7);
    increment_pc(cpu, 2);
}
