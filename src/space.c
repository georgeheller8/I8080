#include <stdlib.h>
#include <stdio.h>
#include "space.h"

uint8_t port_in(space* si, uint8_t port) { // TODO: Handle this

    if (port == 0) {
        return 0x0E;
    }
    else if (port == 1) {
        return si->iports[1];
    }
    else if (port == 2) {
        return si->iports[2];
    }
    else if (port == 3) {
        // Shift register result
        return (si->shift >> (8-(si->oports[2] & 0x07))) & 0xFF;
    }

    return 0;

}

void port_out(space* si, uint8_t port, uint8_t value) { // TODO: Handle this
    if (port < 2) return;
    
    if (port == 2) {
        si->oports[2] = value;
    }
    else if (port == 3) {
        si->oports[3] = value;
    }
    else if (port == 4) {
        si->shift = ((uint16_t)value << 8) | (si->shift >> 8);
    }
    else if (port == 5) {
        si->oports[5] = value;
    }
    else if (port == 6) {
        si->oports[6] = value; // unused
    }
}

void init_space(space* si) {

    si->shift = 0;

    for (size_t i = 0; i < 4; ++i) {
        si->iports[i] = 0;
    }
    for (size_t i = 0; i < 7; ++i) {
        si->oports[i] = 0;
    }

    si->iports[1] = 0x08;

    si->port_in = port_in;
    si->port_out = port_out;
}