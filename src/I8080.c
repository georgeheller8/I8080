#include "I8080.h"
#include "util.h"
#include "opcodes/opcodes.h"
#include <stdlib.h>
#include <stdio.h>

void init_cpu(I8080* cpu, space* space_pointer) {

    cpu->program_counter = 0x0100; // needs to change
    cpu->stack_pointer = 0xF000; // also needs to change
    cpu->cycles = 0;
    cpu->opcode = 0;
    cpu->INTE = 0;
    cpu->HALT = 0;

    for (size_t i = 0; i < 65536; ++i) {
        (cpu->memory)[i] = 0;
    }
    for (size_t i = 0; i < 8; ++i) {
        (cpu->registers)[i] = 0;
    }

    (cpu->registers)[7] |= (1 << 1);

    cpu->si = space_pointer;

}

void (*dispatch[256])(I8080* cpu) =
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

    // printf("%04X  %02X  A=%02X BC=%04X DE=%04X HL=%04X SP=%04X  %c%c%c%c%c\n",
    //     cpu->program_counter, cpu->memory[cpu->program_counter],
    //     cpu->registers[6], BC, DE, HL, cpu->stack_pointer,
    //     SIGN?'S':'.', ZERO?'Z':'.', AUX_CARRY?'A':'.',
    //     PARITY?'P':'.', CARRY?'C':'.');

    cpu->opcode = (cpu->memory)[cpu->program_counter];

    dispatch[cpu->opcode](cpu);

    // check for interrupts

}
