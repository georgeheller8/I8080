#include "space.h"

uint8_t port_in(space* device, uint8_t port) { // TODO: Handle this
    return device->iports[port];
}

void port_out(space* device, uint8_t port, uint8_t value) { // TODO: Handle this
    device->oports[port] = value;
}

void init_space(space* si);