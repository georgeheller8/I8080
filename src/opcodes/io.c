// INPUT/OUTPUT INSTRUCTIONS

#include "util.h"
#include "opcodes.h"

// Input
void IN(I8080* cpu) {

    uint8_t device = (cpu->memory)[cpu->program_counter+1];

    // read byte of data from device to accumulator

    increment_pc(cpu, 2);
    increment_cycles(cpu, 10);
}

// Output
void OUT(I8080* cpu) {

    uint8_t device = (cpu->memory)[cpu->program_counter+1];

    // write byte of data from accumulator to device

    increment_pc(cpu, 2);
    increment_cycles(cpu, 10);
}
