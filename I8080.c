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
    NOP, LXI, STAX, INX, INR, DCR, MVI, RLC, NOP, DAD, LDAX, DCX, INR, DCR, MVI, RRC,
    NOP, LXI, STAX, INX, INR, DCR, MVI, RAL, NOP, DAD, LDAX, DCX, INR, DCR, MVI, RAR,
    NOP, LXI, SHLD, INX, INR, DCR, MVI, DAA, NOP, DAD, LHLD, DCX, INR, DCR, MVI, CMA,
    NOP, LXI, STA,  INX, INR, DCR, MVI, STC, NOP, DAD, LDA,  DCX, INR, DCR, MVI, CMC,
    [0x40 ... 0x7f] = MOV,
    [0x76] = HLT,
    [0x80 ... 0x87] = ADD,
    [0x88 ... 0x8F] = ADC,
    [0x90 ... 0x97] = SUB,
    [0x98 ... 0x9F] = SBB,
    [0xA0 ... 0xA7] = ANA,
    [0xA8 ... 0xAF] = XRA,
    [0xB0 ... 0xB7] = ORA,
    [0xB8 ... 0xBF] = CMP,
    RNZ, POP, JNZ, JMP,  CNZ, PUSH, ADI, RST, RZ,  RET,  JZ,  JMP,  CZ,  CALL, ACI, RST,
    RNC, POP, JNC, OUT,  CNC, PUSH, SUI, RST, RC,  RET,  JC,  IN,   CC,  CALL, SBI, RST,
    RPO, POP, JPO, XTHL, CPO, PUSH, ANI, RST, RPE, PCHL, JPE, XCHG, CPE, CALL, XRI, RST,
    RP,  POP, JP,  DI,   CP,  PUSH, ORI, RST, RM,  SPHL, JM,  EI,   CM,  CALL, CPI, RST,
};

void cycle(I8080* cpu) {

    cpu->opcode = (cpu->memory)[cpu->program_counter];
    
    dispatch[cpu->opcode](cpu);


    // check for interrupts

}

int main() {
    return 0;
}