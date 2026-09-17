#ifndef SPACE_H
#define SPACE_H

#include "I8080.h"

// Hardware interface for space invaders

typedef struct {
    I8080* cpu;
    uint8_t iports[4];
    uint8_t oports[5];
} space;

uint8_t port_in(space* device, uint8_t port);
void port_out(space* device, uint8_t port, uint8_t value);
void init_space(space* si);

#endif