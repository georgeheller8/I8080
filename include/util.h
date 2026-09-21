#ifndef UTIL_H
#define UTIL_H

// Shorthand register/flag accessors and the helper functions in util.c, shared
// by every opcode implementation in opcodes/. Internal to the emulator core --
// not included by main.c.

#include "I8080.h"

#define ACC (cpu->registers)[6]

#define BC (((uint16_t)((cpu->registers)[0]) << 8) | (cpu->registers)[1])
#define DE (((uint16_t)((cpu->registers)[2]) << 8) | (cpu->registers)[3])
#define HL (((uint16_t)((cpu->registers)[4]) << 8) | (cpu->registers)[5])
#define PSW (((uint16_t)((cpu->registers)[6]) << 8) | (cpu->registers)[7])

#define SIGN (((cpu->registers)[7] & (1 << 7)) >> 7)
#define ZERO (((cpu->registers)[7] & (1 << 6)) >> 6)
#define PARITY (((cpu->registers)[7] & (1 << 2)) >> 2)
#define AUX_CARRY (((cpu->registers)[7] & (1 << 4)) >> 4)
#define CARRY ((cpu->registers)[7] & 1)

void push_to_stack(I8080* cpu, uint8_t reg);
void pop_from_stack(I8080* cpu, uint8_t reg);
void increment_cycles(I8080* cpu, uint8_t num);
void increment_pc(I8080* cpu, uint8_t num);

// flag checkers

void set_carry(I8080* cpu, uint8_t num);
void set_aux_carry(I8080* cpu, uint8_t num);
void set_zero(I8080* cpu, uint8_t num);
void set_sign(I8080* cpu, uint8_t num);
void set_parity(I8080* cpu, uint8_t num);

void update_zero(I8080* cpu, uint8_t result);
void update_sign(I8080* cpu, uint8_t result);
void update_carry(I8080* cpu, uint8_t A, uint8_t B, uint8_t OP);
void update_aux_carry(I8080* cpu, uint8_t A, uint8_t B, uint8_t OP);
void update_parity(I8080* cpu, uint8_t result);

#endif // UTIL_H
