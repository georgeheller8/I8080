// NOP INSTRUCTION

#include "util.h"
#include "opcodes.h"

// No operation
void NOP(I8080* cpu) {
    increment_pc(cpu, 1);
    increment_cycles(cpu, 4);
}
