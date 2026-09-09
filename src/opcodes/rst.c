// RST INSTRUCTION

#include "util.h"
#include "opcodes.h"

// Reset
void RST(I8080* cpu) {
    uint16_t EXP = (cpu->opcode & 0x38);
    increment_pc(cpu, 1);
    push_to_stack(cpu, 6);
    cpu->program_counter = EXP;
    increment_cycles(cpu, 11);
}
