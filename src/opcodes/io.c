// INPUT/OUTPUT INSTRUCTIONS

#include "util.h"
#include "opcodes.h"

// Ports are only wired up in the Space Invaders build (SPACE_INVADERS). The
// bare CPU build has nothing attached: IN reads 0 and OUT is ignored.

// Input
void IN(I8080* cpu) {

    uint8_t device = (cpu->memory)[cpu->program_counter+1];

#ifdef SPACE_INVADERS
    ACC = cpu->si->port_in(cpu->si, device);
#else
    (void)device; // no devices attached to the bare CPU
    ACC = 0;
#endif

    increment_pc(cpu, 2);
    increment_cycles(cpu, 10);
}

// Output
void OUT(I8080* cpu) {

    uint8_t device = (cpu->memory)[cpu->program_counter+1];

#ifdef SPACE_INVADERS
    cpu->si->port_out(cpu->si, device, ACC);
#else
    (void)device; // no devices attached to the bare CPU
#endif

    increment_pc(cpu, 2);
    increment_cycles(cpu, 10);
}