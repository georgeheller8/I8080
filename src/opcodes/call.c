// CALL INSTRUCTIONS

#include "util.h"
#include "opcodes.h"

// Call
void CALL(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    increment_pc(cpu, 3);
    push_to_stack(cpu, 6);
    cpu->program_counter = address;
    increment_cycles(cpu, 17);
}

// Call if Carry
void CC(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    increment_pc(cpu, 3);
    if (CARRY) {
        push_to_stack(cpu, 6);
        cpu->program_counter = address;
        increment_cycles(cpu, 17);
    }
    else increment_cycles(cpu, 11);
}

// Call if No Carry
void CNC(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    increment_pc(cpu, 3);
    if (!CARRY) {
        push_to_stack(cpu, 6);
        cpu->program_counter = address;
        increment_cycles(cpu, 17);
    }
    else increment_cycles(cpu, 11);
}

// Call if Zero
void CZ(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    increment_pc(cpu, 3);
    if (ZERO) {
        push_to_stack(cpu, 6);
        cpu->program_counter = address;
        increment_cycles(cpu, 17);
    }
    else increment_cycles(cpu, 11);
}

// Call if Not Zero
void CNZ(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    increment_pc(cpu, 3);
    if (!ZERO) {
        push_to_stack(cpu, 6);
        cpu->program_counter = address;
        increment_cycles(cpu, 17);
    }
    else increment_cycles(cpu, 11);
}

// Call if Minus
void CM(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    increment_pc(cpu, 3);
    if (SIGN) {
        push_to_stack(cpu, 6);
        cpu->program_counter = address;
        increment_cycles(cpu, 17);
    }
    else increment_cycles(cpu, 11);
}

// Call if Plus
void CP(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    increment_pc(cpu, 3);
    if (!SIGN) {
        push_to_stack(cpu, 6);
        cpu->program_counter = address;
        increment_cycles(cpu, 17);
    }
    else increment_cycles(cpu, 11);
}

// Call if Parity Even
void CPE(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    increment_pc(cpu, 3);
    if (PARITY) {
        push_to_stack(cpu, 6);
        cpu->program_counter = address;
        increment_cycles(cpu, 17);
    }
    else increment_cycles(cpu, 11);
}

// Call if Parity Odd
void CPO(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    increment_pc(cpu, 3);
    if (!PARITY) {
        push_to_stack(cpu, 6);
        cpu->program_counter = address;
        increment_cycles(cpu, 17);
    }
    else increment_cycles(cpu, 11);
}
