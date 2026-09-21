#ifndef SPACE_H
#define SPACE_H

#include <stdint.h>

typedef struct space {
    uint8_t iports[4];
    uint8_t oports[7];
    uint16_t shift;
    uint8_t (*port_in)(struct space* si, uint8_t port);
    void (*port_out)(struct space* si, uint8_t port, uint8_t value);
} space;

uint8_t port_in(space* device, uint8_t port);
void port_out(space* device, uint8_t port, uint8_t value);
void init_space(space* si);

#endif