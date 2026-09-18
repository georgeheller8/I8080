#include <stdlib.h>
#include "space.h"

uint8_t port_in(space* si, uint8_t port) { // TODO: Handle this
    return si->iports[port];
}

void port_out(space* si, uint8_t port, uint8_t value) { // TODO: Handle this
    si->oports[port] = value;
}

void init_space(space* si) {

    for (size_t i = 0; i < 4; ++i) {
        si->iports[i] = 0;
    }
    for (size_t i = 0; i < 7; ++i) {
        si->oports[i] = 0;
    }

    si->port_in = port_in;
    si->port_out = port_out;
}