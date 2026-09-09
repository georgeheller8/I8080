// HLT INSTRUCTION

#include "util.h"
#include "opcodes.h"

// Halt
void HLT(I8080* cpu) {

    cpu->HALT = 1;

    increment_pc(cpu, 1);
    increment_cycles(cpu, 7);
}
