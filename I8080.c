#include "I8080.h"
#include <stdlib.h>

void push_to_stack(I8080* cpu, uint8_t reg) {

    // Register options: 0 is BC pair, 2, is DE pair, 4 is HL pair, 6 is program counter

    uint8_t MSB, LSB;

    if (reg == 6) {
        MSB = ((cpu->program_counter) >> 8);
        LSB = ((cpu->program_counter) & 0x00FF);
    }
    else {
        MSB = (cpu->registers)[reg];
        LSB = (cpu->registers)[reg+1];
    }

    (cpu->memory)[--(cpu->stack_pointer)] = MSB;
    (cpu->memory)[--(cpu->stack_pointer)] = LSB;

}

void pop_from_stack(I8080* cpu, uint8_t reg) {

    // LSB is at address of stack pointer, MSB is at address of stack pointer + 1

    if (reg == 6) {
        (cpu->program_counter) = (cpu->memory)[(cpu->stack_pointer)++];
        (cpu->program_counter) |= ((uint16_t)((cpu->memory)[(cpu->stack_pointer)++]) << 8);
    }
    else {
        (cpu->registers)[reg+1] = (cpu->memory)[(cpu->stack_pointer)++];
        (cpu->registers)[reg] = (cpu->memory)[(cpu->stack_pointer)++];
    }

}

void init_cpu(I8080* cpu) {

    cpu->program_counter = 0; // needs to change
    cpu->stack_pointer = 0; // also needs to change
    cpu->temp_accumulator = 0;
    cpu->TMP = 0;
    cpu->cycles = 0;
    cpu->opcode = 0;

    for (size_t i = 0; i < 65536; ++i) {
        (cpu->memory)[i] = 0;
    }
    for (size_t i = 0; i < 8; ++i) {
        (cpu->registers)[i] = 0;
    }
    for (size_t i = 0; i < 2; ++i) {
        (cpu->wz)[i] = 0;
    }
    
}

void increment_cycles(I8080* cpu, uint8_t num) {
    cpu->cycles += num;
}

const void (*dispatch[256])(I8080* cpu) = 
{
 // x0    x1    x2    x3    x4    x5    x6    x7    x8    x9    xA    xB    xC    xD    xE    xF
    NOP,  LXI,  STAX, INX,  INR,  DCR,  MVI,  RLC,  NOP,  DAD,  LDAX, DCX,  INR,  DCR,  MVI,  RRC, // 0x
    NOP,  LXI,  STAX, INX,  INR,  DCR,  MVI,  RAL,  NOP,  DAD,  LDAX, DCX,  INR,  DCR,  MVI,  RAR, // 1x
    NOP,  LXI,  SHLD, INX,  INR,  DCR,  MVI,  DAA,  NOP,  DAD,  LHLD, DCX,  INR,  DCR,  MVI,  CMA, // 2x
    NOP,  LXI,  STA,  INX,  INR,  DCR,  MVI,  STC,  NOP,  DAD,  LDA,  DCX,  INR,  DCR,  MVI,  CMC, // 3x
    MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV, // 4x
    MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV, // 5x
    MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV, // 6x
    MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  HLT,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV,  MOV, // 7x
    ADD,  ADD,  ADD,  ADD,  ADD,  ADD,  ADD,  ADD,  ADC,  ADC,  ADC,  ADC,  ADC,  ADC,  ADC,  ADC, // 8x
    SUB,  SUB,  SUB,  SUB,  SUB,  SUB,  SUB,  SUB,  SBB,  SBB,  SBB,  SBB,  SBB,  SBB,  SBB,  SBB, // 9x
    ANA,  ANA,  ANA,  ANA,  ANA,  ANA,  ANA,  ANA,  XRA,  XRA,  XRA,  XRA,  XRA,  XRA,  XRA,  XRA, // Ax
    ORA,  ORA,  ORA,  ORA,  ORA,  ORA,  ORA,  ORA,  CMP,  CMP,  CMP,  CMP,  CMP,  CMP,  CMP,  CMP, // Bx
    RNZ,  POP,  JNZ,  JMP,  CNZ,  PUSH, ADI,  RST,  RZ,   RET,  JZ,   JMP,  CZ,   CALL, ACI,  RST, // Cx
    RNC,  POP,  JNC,  OUT,  CNC,  PUSH, SUI,  RST,  RC,   RET,  JC,   IN,   CC,   CALL, SBI,  RST, // Dx
    RPO,  POP,  JPO,  XTHL, CPO,  PUSH, ANI,  RST,  RPE,  PCHL, JPE,  XCHG, CPE,  CALL, XRI,  RST, // Ex
    RP,   POP,  JP,   DI,   CP,   PUSH, ORI,  RST,  RM,   SPHL, JM,   EI,   CM,   CALL, CPI,  RST, // Fx
    
};

void cycle(I8080* cpu) {

    cpu->opcode = (cpu->memory)[cpu->program_counter];

    dispatch[cpu->opcode](cpu);


    // check for interrupts

}

int main() {
    return 0;
}