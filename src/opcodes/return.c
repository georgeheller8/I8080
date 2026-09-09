// RETURN INSTRUCTIONS

#include "util.h"
#include "opcodes.h"

// Return
void RET(I8080* cpu) {
    pop_from_stack(cpu, 6);
    increment_cycles(cpu, 10);
}

// Return if Carry
void RC(I8080* cpu) {
    if (CARRY) {
        pop_from_stack(cpu, 6);
        increment_cycles(cpu, 11);
    }
    else {
        increment_cycles(cpu, 5);
        increment_pc(cpu, 1);
    }
}

// Return if Not Carry
void RNC(I8080* cpu) {
    if (!CARRY) {
        pop_from_stack(cpu, 6);
        increment_cycles(cpu, 11);
    }
    else {
        increment_cycles(cpu, 5);
        increment_pc(cpu, 1);
    }
}

// Return if Zero
void RZ(I8080* cpu) {
    if (ZERO) {
        pop_from_stack(cpu, 6);
        increment_cycles(cpu, 11);
    }
    else {
        increment_cycles(cpu, 5);
        increment_pc(cpu, 1);
    }
}

// Return if Not Zero
void RNZ(I8080* cpu) {
    if (!ZERO) {
        pop_from_stack(cpu, 6);
        increment_cycles(cpu, 11);
    }
    else {
        increment_cycles(cpu, 5);
        increment_pc(cpu, 1);
    }
}

// Return if Minus
void RM(I8080* cpu) {
    if (SIGN) {
        pop_from_stack(cpu, 6);
        increment_cycles(cpu, 11);
    }
    else {
        increment_cycles(cpu, 5);
        increment_pc(cpu, 1);
    }
}

// Return if Positive
void RP(I8080* cpu) {
    if (!SIGN) {
        pop_from_stack(cpu, 6);
        increment_cycles(cpu, 11);
    }
    else {
        increment_cycles(cpu, 5);
        increment_pc(cpu, 1);
    }
}

// Return if Parity Even
void RPE(I8080* cpu) {
    if (PARITY) {
        pop_from_stack(cpu, 6);
        increment_cycles(cpu, 11);
    }
    else {
        increment_cycles(cpu, 5);
        increment_pc(cpu, 1);
    }
}

// Return if Parity Odd
void RPO(I8080* cpu) {
    if (!PARITY) {
        pop_from_stack(cpu, 6);
        increment_cycles(cpu, 11);
    }
    else {
        increment_cycles(cpu, 5);
        increment_pc(cpu, 1);
    }
}
