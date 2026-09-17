// INPUT/OUTPUT INSTRUCTIONS

#include "util.h"
#include "opcodes.h"
#include "space.h"

// will likely change port_in and port_out to be declared in opcodes.h,
// and then include that in space.c and define the functions there.
// allows for implementations to be hardware-specific and for 
// multiple executables to be compiled without needing to
// use preprocessor instructions

// Input
void IN(I8080* cpu) {

    uint8_t device = (cpu->memory)[cpu->program_counter+1];

    ACC = port_in(cpu, device);

    increment_pc(cpu, 2);
    increment_cycles(cpu, 10);
}

// Output
void OUT(I8080* cpu) {

    uint8_t device = (cpu->memory)[cpu->program_counter+1];

    port_out(cpu, device, ACC);

    increment_pc(cpu, 2);
    increment_cycles(cpu, 10);
}