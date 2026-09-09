// INTERRUPT FLIP-FLOP INSTRUCTIONS

#include "util.h"
#include "opcodes.h"

// Enable Interrupts
void EI(I8080* cpu) {
    cpu->INTE = 1;
    increment_pc(cpu, 1);
    increment_cycles(cpu, 4);
}

// Disable Interrupts
void DI(I8080* cpu) {
    cpu->INTE = 0;
    increment_pc(cpu, 1);
    increment_cycles(cpu, 4);
}
