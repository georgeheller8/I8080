#include "I8080.h"
#include <stdlib.h> 

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
    cpu->INTE = 0;
    cpu->HALT = 0;

    for (size_t i = 0; i < 65536; ++i) {
        (cpu->memory)[i] = 0;
    }
    for (size_t i = 0; i < 8; ++i) {
        (cpu->registers)[i] = 0;
    }
    for (size_t i = 0; i < 2; ++i) {
        (cpu->wz)[i] = 0;
    }

    (cpu->registers)[7] |= (1 << 1);
    
}

void increment_cycles(I8080* cpu, uint8_t num) {
    cpu->cycles += num;
}

void increment_pc(I8080* cpu, uint8_t num) {
    cpu->program_counter += num;
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

    cpu->opcode = (cpu->memory)[cpu->program_counter];

    dispatch[cpu->opcode](cpu);

    // check for interrupts

}

int main() {
    return 0;
}

// util functions

void set_zero(I8080* cpu, uint8_t num) {
    (cpu->registers)[7] &= 0xBF;
    (cpu->registers)[7] |= (num << 6);
}

void set_sign(I8080* cpu, uint8_t num) {
    (cpu->registers)[7] &= 0x7F;
    (cpu->registers)[7] |= (num << 7);
}

void set_carry(I8080* cpu, uint8_t num) {
    (cpu->registers)[7] &= 0xFE;
    (cpu->registers)[7] |= num;
}

void set_aux_carry(I8080* cpu, uint8_t num) {
    (cpu->registers)[7] &= 0xEF;
    (cpu->registers)[7] |= (num << 4);
}

void set_parity(I8080* cpu, uint8_t num) {
    (cpu->registers)[7] &= 0xFB;
    (cpu->registers)[7] |= (num << 2);
}

void update_zero(I8080* cpu, uint8_t result) {
    (cpu->registers)[7] &= 0xBF;
    (cpu->registers)[7] |= ((result == 0) << 6);
}

void update_sign(I8080* cpu, uint8_t result) {
    (cpu->registers)[7] &= 0x7F;
    (cpu->registers)[7] |= ((result & 0x80) >> 7);
}

void update_carry(I8080* cpu, uint8_t A, uint8_t B, uint8_t OP) {
    // OP = 0 for addition, 1 for subtraction
    // In subtraction cases, B is being subtracted from A
    uint16_t check;
    check = A + B;

    (cpu->registers)[7] &= 0xFE;

    if (OP == 0) {
        (cpu->registers)[7] |= (check >= 256);
    }
    else if (OP == 1) {
        (cpu->registers)[7] |= (A < B);
    }
    else if (OP == 2) {
        check += CARRY;
        (cpu->registers)[7] |= (check >= 256);
    }

}

void update_aux_carry(I8080* cpu, uint8_t A, uint8_t B, uint8_t OP) {
    // OP = 0 for addition, 1 for subtraction

    uint8_t Al = A & 0x0F;
    uint8_t Bl = B & 0x0F;

    uint8_t check = Al + Bl;

    (cpu->registers)[7] &= 0xEF;

    if (OP == 0) {
        (cpu->registers)[7] |= ((check >= 16) << 4);
    }
    else if (OP == 2) {
        (cpu->registers)[7] |= ((Al < Bl) << 4);
    }
    else if (OP == 3) {
        check += AUX_CARRY;
        (cpu->registers)[7] |= ((check >= 16) << 4);
    }

}

void update_parity(I8080* cpu, uint8_t result) {
    (cpu->registers)[7] &= 0xFB;
    (cpu->registers)[7] |= (((__builtin_popcount(result) % 2) == 0) << 2);
}


// Set carry
void STC(I8080* cpu) {
    (cpu->registers)[7] |= 0x01;
    increment_pc(cpu, 1);
    increment_cycles(cpu, 4);
}

// Complement carry
void CMC(I8080* cpu) {
    (cpu->registers)[7] ^= 0x01;
    increment_pc(cpu, 1);
    increment_cycles(cpu, 4);
}

// Increment register
void INR(I8080* cpu) {

    uint8_t reg = (cpu->opcode) >> 3;
    uint8_t result;

    if (reg < 6) {
        (cpu->registers)[reg] += 1;
        result = (cpu->registers)[reg];
    }
    else if (reg == 6) {
        (cpu->memory)[((cpu->registers)[5] << 8) | (cpu->registers)[6]] += 1;
        result = (cpu->memory)[((cpu->registers)[5] << 8) | (cpu->registers)[6]];
    }
    else if (reg == 7) {
        (cpu->registers)[6] += 1;
        result = (cpu->registers)[6];
    }

    update_sign(cpu, result);
    update_zero(cpu, result);
    update_parity(cpu, result);
    increment_pc(cpu, 1);
    increment_cycles(cpu, 5);

}

// Decrement register
void DCR(I8080* cpu) {

    uint8_t reg = (cpu->opcode) >> 3;
    uint8_t result;

    if (reg < 6) {
        (cpu->registers)[reg] -= 1;
        result = (cpu->registers)[reg];
    }
    else if (reg == 6) {
        (cpu->memory)[((cpu->registers)[5] << 8) | (cpu->registers)[6]] -= 1;
        result = (cpu->memory)[((cpu->registers)[5] << 8) | (cpu->registers)[6]];
    }
    else if (reg == 7) {
        (cpu->registers)[6] -= 1;
        result =  (cpu->registers)[6];
    }

    update_sign(cpu, result);
    update_zero(cpu, result);
    update_parity(cpu, result);
    increment_pc(cpu, 1);
    increment_cycles(cpu, 5);

}

// Complement accumulator
void CMA(I8080* cpu) {

    (cpu->registers)[6] ^= 0xFF;
    increment_pc(cpu, 1);
    increment_cycles(cpu, 4);
}


// Decimal adjust accumulator
void DAA(I8080* cpu) {
    
    uint8_t LSB = ACC & 0x0F;
    uint8_t MSB = ACC >> 4;

    if (LSB > 9 || AUX_CARRY) {
        update_aux_carry(cpu, ACC, 6, 0);
        ACC += 6;
        MSB = (ACC >> 4);
    } 
    if (MSB > 9 || CARRY) {
        (cpu->registers)[7] |= (ACC + 0x60 > 255);
        ACC += 0x60;
    }

    update_zero(cpu, ACC);
    update_sign(cpu, ACC);
    update_parity(cpu, ACC);
    increment_pc(cpu, 1);
    increment_cycles(cpu, 4);

}

// No operation
void NOP(I8080* cpu) {
    increment_pc(cpu, 1);
    increment_cycles(cpu, 4);
}

// Move
void MOV(I8080* cpu) {

    uint8_t SRC = (cpu->opcode) & 0x07;
    uint8_t DEST = ((cpu->opcode) >> 3) & 0x07;

    uint8_t SRC_v;

    if (SRC <= 5) {
        SRC_v = (cpu->registers)[SRC];
    }
    else if (SRC == 6) {
        SRC_v = (cpu->memory)[HL];
    }
    else if (SRC == 7) {
        SRC_v = (cpu->registers)[6];
    }

    if (DEST <= 5) {
        (cpu->registers)[DEST] = SRC_v;
    }
    else if (DEST == 6) {
        (cpu->memory)[HL] = SRC_v;
    }
    else if (DEST == 7) {
        (cpu->registers)[6] = SRC_v;
    }

    increment_pc(cpu, 1);

    if (SRC != 5 && DEST != 5) {
        increment_cycles(cpu, 5);
    }
    else {
        increment_cycles(cpu, 7);
    }

}

// Store accumulator
void STAX(I8080* cpu) {

    (cpu->memory)[(((cpu->opcode) & 0x10) >> 4) ? DE : BC] = ACC;

    increment_pc(cpu, 1);
    increment_cycles(cpu, 7);

}

// Load accumulator
void LDAX(I8080* cpu) {
    
    ACC = (cpu->memory)[(((cpu->opcode) & 0x10) >> 4) ? DE : BC];

    increment_pc(cpu, 1);
    increment_cycles(cpu, 7);   

}

// ADD Register or Memory to Accumulator
void ADD(I8080* cpu) {

    uint8_t SRC = (cpu->opcode) & 0x07;
    uint8_t SRC_v;

    if (SRC <= 5) {
        SRC_v = (cpu->registers)[SRC];
    }
    else if (SRC == 6) {
        SRC_v = (cpu->memory)[HL];
    }
    else if (SRC == 7) {
        SRC_v = (cpu->registers)[6];
    }

    update_carry(cpu, SRC_v, ACC, 0);
    update_aux_carry(cpu, SRC_v, ACC, 0);

    ACC += SRC_v;

    update_sign(cpu, ACC);
    update_zero(cpu, ACC);
    update_parity(cpu, ACC);

    if (SRC != 5) {
        increment_cycles(cpu, 4);
    }
    else {
        increment_cycles(cpu, 7);
    }

    increment_pc(cpu, 1);

}

// ADD Register or Memory to Accumulator with Carry
void ADC(I8080* cpu) {

    uint8_t SRC = (cpu->opcode) & 0x07;
    uint8_t SRC_v;

    if (SRC <= 5) {
        SRC_v = (cpu->registers)[SRC];
    }
    else if (SRC == 6) {
        SRC_v = (cpu->memory)[HL];
    }
    else if (SRC == 7) {
        SRC_v = (cpu->registers)[6];
    }

    update_carry(cpu, SRC_v, ACC, 2);
    update_aux_carry(cpu, SRC_v, ACC, 2);

    ACC += SRC_v;
    ACC += CARRY;

    update_sign(cpu, ACC);
    update_zero(cpu, ACC);
    update_parity(cpu, ACC);

    if (SRC != 5) {
        increment_cycles(cpu, 4);
    }
    else {
        increment_cycles(cpu, 7);
    }

    increment_pc(cpu, 1);
}

// Subtrack Register or Memory from Accumulator
void SUB(I8080* cpu) {
    uint8_t SRC = (cpu->opcode) & 0x07;
    uint8_t SRC_v;

    if (SRC <= 5) {
        SRC_v = (cpu->registers)[SRC];
    }
    else if (SRC == 6) {
        SRC_v = (cpu->memory)[HL];
    }
    else if (SRC == 7) {
        SRC_v = (cpu->registers)[6];
    }

    update_carry(cpu, ACC, SRC_v, 1);
    update_aux_carry(cpu, ACC, SRC_v, 1);

    ACC -= SRC_v;

    update_sign(cpu, ACC);
    update_zero(cpu, ACC);
    update_parity(cpu, ACC);

    if (SRC != 5) {
        increment_cycles(cpu, 4);
    }
    else {
        increment_cycles(cpu, 7);
    }

    increment_pc(cpu, 1);
}

// subtract with borrow
void SBB(I8080* cpu) {
    uint8_t SRC = (cpu->opcode) & 0x07;
    uint8_t SRC_v;

    if (SRC <= 5) {
        SRC_v = (cpu->registers)[SRC];
    }
    else if (SRC == 6) {
        SRC_v = (cpu->memory)[HL];
    }
    else if (SRC == 7) {
        SRC_v = (cpu->registers)[6];
    }

    update_carry(cpu, ACC, SRC_v + CARRY, 1);
    update_aux_carry(cpu, ACC, SRC_v + CARRY, 1);

    ACC -= (SRC_v + CARRY);

    update_sign(cpu, ACC);
    update_zero(cpu, ACC);
    update_parity(cpu, ACC);

    if (SRC != 5) {
        increment_cycles(cpu, 4);
    }
    else {
        increment_cycles(cpu, 7);
    }

    increment_pc(cpu, 1);
}

// Logical AND acc with memory or register
void ANA(I8080* cpu) {
    uint8_t SRC = (cpu->opcode) & 0x07;
    uint8_t SRC_v;

    if (SRC <= 5) {
        SRC_v = (cpu->registers)[SRC];
    }
    else if (SRC == 6) {
        SRC_v = (cpu->memory)[HL];
    }
    else if (SRC == 7) {
        SRC_v = (cpu->registers)[6];
    }

    ACC &= SRC_v;
    (cpu->registers)[7] &= 0xFE;

    update_sign(cpu, ACC);
    update_zero(cpu, ACC);
    update_parity(cpu, ACC);

    if (SRC != 5) {
        increment_cycles(cpu, 4);
    }
    else {
        increment_cycles(cpu, 7);
    }

    increment_pc(cpu, 1);    

}

// Logical XOR acc with memory or register
void XRA(I8080* cpu) {
     uint8_t SRC = (cpu->opcode) & 0x07;
    uint8_t SRC_v;

    if (SRC <= 5) {
        SRC_v = (cpu->registers)[SRC];
    }
    else if (SRC == 6) {
        SRC_v = (cpu->memory)[HL];
    }
    else if (SRC == 7) {
        SRC_v = (cpu->registers)[6];
    }

    ACC ^= SRC_v;
    (cpu->registers)[7] &= 0xFE;

    // update aux carry?

    update_sign(cpu, ACC);
    update_zero(cpu, ACC);
    update_parity(cpu, ACC);

    if (SRC != 5) {
        increment_cycles(cpu, 4);
    }
    else {
        increment_cycles(cpu, 7);
    }

    increment_pc(cpu, 1);    
}

// Logical OR with memory or register
void ORA(I8080* cpu) {
    uint8_t SRC = (cpu->opcode) & 0x07;
    uint8_t SRC_v;

    if (SRC <= 5) {
        SRC_v = (cpu->registers)[SRC];
    }
    else if (SRC == 6) {
        SRC_v = (cpu->memory)[HL];
    }
    else if (SRC == 7) {
        SRC_v = (cpu->registers)[6];
    }

    ACC |= SRC_v;
    (cpu->registers)[7] &= 0xFE;

    // update aux carry?

    update_sign(cpu, ACC);
    update_zero(cpu, ACC);
    update_parity(cpu, ACC);

    if (SRC != 5) {
        increment_cycles(cpu, 4);
    }
    else {
        increment_cycles(cpu, 7);
    }

    increment_pc(cpu, 1);    
}

// Compare Register or Memory with Accumulator
void CMP(I8080* cpu) {
    uint8_t SRC = (cpu->opcode) & 0x07;
    uint8_t SRC_v;

    if (SRC <= 5) {
        SRC_v = (cpu->registers)[SRC];
    }
    else if (SRC == 6) {
        SRC_v = (cpu->memory)[HL];
    }
    else if (SRC == 7) {
        SRC_v = (cpu->registers)[6];
    }

    uint8_t ACC_v; 
    ACC_v = ACC;
    
    if (ACC_v > SRC_v) {
        (cpu->registers)[7] &= 0xFE;
        (cpu->registers)[7] &= ~(1 << 6);
    }
    else if (ACC_v == SRC_v) {
        (cpu->registers)[7] |= (1 << 6);
    }
    else if (ACC_v < SRC_v) {
        (cpu->registers)[7] |= 0x01;
        (cpu->registers)[7] &= ~(1 << 6);
    }

    update_sign(cpu, ACC);
    update_zero(cpu, ACC);
    update_parity(cpu, ACC);

    if (SRC != 5) {
        increment_cycles(cpu, 4);
    }
    else {
        increment_cycles(cpu, 7);
    }

    increment_pc(cpu, 1);  

}

// Rotate accumulator left
void RLC(I8080* cpu) {
    set_carry(cpu, ACC >> 7);
    ACC = (ACC << 1);
    ACC |= CARRY;

    increment_cycles(cpu, 4);
    increment_pc(cpu, 1);
}

// Rotate accumulator right
void RRC(I8080* cpu) {
    set_carry(cpu, ACC & 1);
    ACC = (ACC >> 1);
    ACC |= (CARRY << 7);

    increment_cycles(cpu, 4);
    increment_pc(cpu, 1);
}

// Rotate accumulator left through carry
void RAL(I8080* cpu) {
    uint8_t CARRY_c = CARRY;
    set_carry(cpu, ACC >> 7);
    ACC = (ACC << 1);
    ACC |= CARRY_c;

    increment_cycles(cpu, 4);
    increment_pc(cpu, 1);
}

// Rotate accumulator right through carry
void RAR(I8080* cpu) {
    uint8_t CARRY_c = CARRY;
    set_carry(cpu, ACC & 0x01);
    ACC = (ACC >> 1);
    ACC |= (CARRY_c << 7);

    increment_cycles(cpu, 4);
    increment_pc(cpu, 1);
}

// Push Data Onto Stack
void PUSH(I8080* cpu) {
    uint8_t REG = (cpu->opcode) >> 4;
    push_to_stack(cpu, REG * 2);

    increment_cycles(cpu, 11);
    increment_pc(cpu, 1);
}

// Pop Data Off Stack
void POP(I8080* cpu) {
    uint8_t REG = (cpu->opcode) >> 4;
    pop_from_stack(cpu, REG * 2);

    increment_cycles(cpu, 10);
    increment_pc(cpu, 1);
}

// Double ADD
void DAD(I8080* cpu) {
    uint8_t REG = (cpu->opcode) >> 4;
    uint16_t REG_v;
    uint16_t val;

    switch (REG) {
        case 0:
            REG_v = BC;
            break;
        case 1:
            REG_v = DE;
            break;
        case 2:
            REG_v = HL;
            break;
        case 3:
            REG_v = (cpu->stack_pointer);
            break;
    }

    val = HL + REG_v;
    uint8_t H, L;
    H = (val >> 8);
    L = (val & 0x00FF);

    (cpu->registers)[4] = H;
    (cpu->registers)[5] = L;

    update_carry(cpu, H, L, 0);

    increment_cycles(cpu, 10);
    increment_pc(cpu, 1);    
}

// Increment register pair
void INX(I8080* cpu) {
    uint8_t REG = (cpu->opcode) >> 4;
    uint16_t REG_v;
    uint16_t val;
    uint8_t H, L;

    switch (REG) {
        case 0:
            REG_v = BC+1;
            H = (val >> 8);
            L = (val & 0x00FF);
            (cpu->registers)[0] = H;
            (cpu->registers)[1] = L;
            break;
        case 1:
            REG_v = DE+1;
            H = (val >> 8);
            L = (val & 0x00FF);
            (cpu->registers)[0] = H;
            (cpu->registers)[1] = L;
            break;
        case 2:
            REG_v = HL+1;
            H = (val >> 8);
            L = (val & 0x00FF);
            (cpu->registers)[0] = H;
            (cpu->registers)[1] = L;
            break;
        case 3:
            (cpu->stack_pointer) += 1;
            break;
    }

    increment_cycles(cpu, 5);
    increment_pc(cpu, 1);    
}

// Decrement register pair
void DCX(I8080* cpu) {
    uint8_t REG = (cpu->opcode) >> 4;
    uint16_t REG_v;
    uint16_t val;
    uint8_t H, L;

    switch (REG) {
        case 0:
            REG_v = BC-1;
            H = (val >> 8);
            L = (val & 0x00FF);
            (cpu->registers)[0] = H;
            (cpu->registers)[1] = L;
            break;
        case 1:
            REG_v = DE-1;
            H = (val >> 8);
            L = (val & 0x00FF);
            (cpu->registers)[0] = H;
            (cpu->registers)[1] = L;
            break;
        case 2:
            REG_v = HL-1;
            H = (val >> 8);
            L = (val & 0x00FF);
            (cpu->registers)[0] = H;
            (cpu->registers)[1] = L;
            break;
        case 3:
            (cpu->stack_pointer) -= 1;
            break;
    }

    increment_cycles(cpu, 5);
    increment_pc(cpu, 1);    
}

// Exchange registers
void XCHG(I8080* cpu) {
    uint16_t temp = HL;

    (cpu->registers)[4] = (cpu->registers)[2];
    (cpu->registers)[5] = (cpu->registers)[3];

    (cpu->registers)[2] = (temp >> 8);
    (cpu->registers)[3] = (temp & 0x00FF);

    increment_cycles(cpu, 4);
    increment_pc(cpu, 1);  
}


// Exchange stack
void XTHL(I8080* cpu) {
    (cpu->registers)[5] = (cpu->memory)[cpu->stack_pointer];
    (cpu->registers)[4] = (cpu->memory)[cpu->stack_pointer+1];

    increment_cycles(cpu, 18);
    increment_pc(cpu, 1);
}

// Load SP from H and L
void SPHL(I8080* cpu) {
    (cpu->stack_pointer) = HL;

    increment_cycles(cpu, 5);
    increment_pc(cpu, 1);
}

// Load immediate data
void LXI(I8080* cpu) {
    uint8_t reg = (cpu->opcode >> 3);
    uint8_t MSB = (cpu->memory)[cpu->program_counter + 2];
    uint8_t LSB = (cpu->memory)[cpu->program_counter + 1];

    switch (reg) {
        case 0: // BC
            (cpu->registers)[0] = MSB;
            (cpu->registers)[1] = LSB;
            break;
        case 1: // DE
            (cpu->registers)[2] = MSB;
            (cpu->registers)[3] = LSB;
            break;
        case 2: // HL
            (cpu->registers)[4] = MSB;
            (cpu->registers)[5] = LSB;
            break;
        case 3: // SP
            cpu->stack_pointer = ((uint16_t)MSB << 8) | LSB;
            break;
    }

    increment_pc(cpu, 3);
    increment_cycles(cpu, 10);

}

// Move Immediate Data
void MVI(I8080* cpu) {
    uint8_t reg = (cpu->opcode >> 3);
    uint8_t data = (cpu->memory)[cpu->program_counter + 1];

    if (reg < 6) {
        (cpu->registers)[reg] = data;
    }
    else if (reg == 6) {
        (cpu->memory)[HL] = data;
    }
    else if (reg == 7) {
        (cpu->registers)[6] = data;
    }

    if (reg != 6) {
        increment_cycles(cpu, 7);
    }
    else {
        increment_cycles(cpu, 10);
    }

    increment_pc(cpu, 2);
}

// Add Immediate to Accumulator
void ADI(I8080* cpu) {
    uint8_t data = (cpu->memory)[cpu->program_counter + 1];

    update_carry(cpu, data, ACC, 0);
    update_aux_carry(cpu, data, ACC, 0);

    ACC += data;

    update_zero(cpu, ACC);
    update_sign(cpu, ACC);
    update_parity(cpu, ACC);

    increment_cycles(cpu, 7);
    increment_pc(cpu, 2);
}

// Add Immediate with Carry
void ACI(I8080* cpu) {
    uint8_t data = (cpu->memory)[cpu->program_counter + 1];

    set_carry(cpu, (uint16_t)(data + ACC + CARRY) > 255);
    set_aux_carry(cpu, (data & 0x0F) + (ACC & 0x0F) + CARRY > 15);

    ACC += (data + CARRY);

    update_zero(cpu, ACC);
    update_parity(cpu, ACC);
    update_sign(cpu, ACC);

    increment_cycles(cpu, 7);
    increment_pc(cpu, 2);
    
}

// Subtract Immediate From Accumulator
void SUI(I8080* cpu) {
    uint8_t data = (cpu->memory)[cpu->program_counter + 1];

    update_carry(cpu, ACC, data, 1);
    update_aux_carry(cpu, ACC, data, 1);

    ACC -= data;

    update_zero(cpu, ACC);
    update_parity(cpu, ACC);
    update_sign(cpu, ACC);

    increment_cycles(cpu, 7);
    increment_pc(cpu, 2);
}

// Subtract Immediate from Accumulator with Borrow
void SBI(I8080* cpu) {
    uint8_t data = (cpu->memory)[cpu->program_counter + 1];

    update_carry(cpu, ACC, (data+CARRY), 1);
    update_aux_carry(cpu, ACC, (data+CARRY), 1);

    ACC -= (data+CARRY);

    update_zero(cpu, ACC);
    update_parity(cpu, ACC);
    update_sign(cpu, ACC);

    increment_cycles(cpu, 7);
    increment_pc(cpu, 2);    
}

// And Immediate with Accumulator
void ANI(I8080* cpu) {
    uint8_t data = (cpu->memory)[cpu->program_counter + 1];

    ACC &= data;

    set_carry(cpu, 0);
    set_aux_carry(cpu, 0);
    update_zero(cpu, ACC);
    update_sign(cpu, ACC);
    update_parity(cpu, ACC);

    increment_cycles(cpu, 7);
    increment_pc(cpu, 2); 
}

// XOR Immediate with Accumulator
void XRI(I8080* cpu) {
    uint8_t data = (cpu->memory)[cpu->program_counter + 1];

    ACC ^= data;

    set_carry(cpu, 0);
    set_aux_carry(cpu, 0);
    update_zero(cpu, ACC);
    update_sign(cpu, ACC);
    update_parity(cpu, ACC);

    increment_cycles(cpu, 7);
    increment_pc(cpu, 2); 
}

// Or Immediate with Accumulator
void ORI(I8080* cpu) {
    uint8_t data = (cpu->memory)[cpu->program_counter + 1];

    ACC |= data;

    set_carry(cpu, 0);
    set_aux_carry(cpu, 0);
    update_zero(cpu, ACC);
    update_sign(cpu, ACC);
    update_parity(cpu, ACC);

    increment_cycles(cpu, 7);
    increment_pc(cpu, 2); 
}

// Compare Immediate with Accumulator
void CPI(I8080* cpu) {
    uint8_t data = (cpu->memory)[cpu->program_counter + 1];

    if (ACC > data) {
        (cpu->registers)[7] &= 0xFE;
        (cpu->registers)[7] &= ~(1 << 6);
    }
    else if (ACC == data) {
        (cpu->registers)[7] |= (1 << 6);
    }
    else if (ACC < data) {
        (cpu->registers)[7] |= 0x01;
        (cpu->registers)[7] &= ~(1 << 6);
    }

    update_sign(cpu, ACC);
    update_zero(cpu, ACC);
    update_parity(cpu, ACC);

    increment_cycles(cpu, 7);
    increment_pc(cpu, 2); 
}

// Store Accumulator Direct
void STA(I8080* cpu) {
    uint16_t LSB = (cpu->memory)[cpu->program_counter + 1];
    uint16_t MSB = (cpu->memory)[cpu->program_counter + 2];

    (cpu->memory)[(MSB << 8) | LSB] = ACC;

    increment_cycles(cpu, 13);
    increment_pc(cpu, 3); 
}

// Load Accumulator Direct
void LDA(I8080* cpu) {
    uint16_t LSB = (cpu->memory)[cpu->program_counter + 1];
    uint16_t MSB = (cpu->memory)[cpu->program_counter + 2];

    ACC = (cpu->memory)[(MSB << 8) | LSB];

    increment_cycles(cpu, 13);
    increment_pc(cpu, 3); 
}

// Store H and L direct
void SHLD(I8080* cpu) {
    uint16_t LSB = (cpu->memory)[cpu->program_counter + 1];
    uint16_t MSB = (cpu->memory)[cpu->program_counter + 2];

    (cpu->memory)[(MSB << 8) | LSB] = (cpu->registers)[5];
    (cpu->memory)[((MSB << 8) | LSB) + 1] = (cpu->registers)[4];

    increment_cycles(cpu, 16);
    increment_pc(cpu, 3);
}

// Load H and L Direct
void LHLD(I8080* cpu) {
    uint16_t LSB = (cpu->memory)[cpu->program_counter + 1];
    uint16_t MSB = (cpu->memory)[cpu->program_counter + 2];
    
    (cpu->registers)[5] = (cpu->memory)[(MSB << 8) | LSB];
    (cpu->registers)[4] = (cpu->memory)[((MSB << 8) | LSB) + 1];

    increment_cycles(cpu, 16);
    increment_pc(cpu, 3);
}

// Load Program Counter
void PCHL(I8080* cpu) {
    (cpu->program_counter) = HL;

    increment_cycles(cpu, 5);
}

// Jump
void JMP(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    cpu->program_counter = address;
    increment_cycles(cpu, 10);
}

// Jump if Carry
void JC(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    if (CARRY) cpu->program_counter = address;
    increment_cycles(cpu, 10);
}

// Jump if No Carry
void JNC(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    if (!CARRY) cpu->program_counter = address;
    increment_cycles(cpu, 10);
}

// Jump if Zero
void JZ(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    if (ZERO) cpu->program_counter = address;
    increment_cycles(cpu, 10);
}

// Jump if Not Zero
void JNZ(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    if (!ZERO) cpu->program_counter = address;
    increment_cycles(cpu, 10);
}

// Jump if Minus
void JM(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    if (SIGN) cpu->program_counter = address;
    increment_cycles(cpu, 10);
}

// Jump if Positive
void JP(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    if (!SIGN) cpu->program_counter = address;
    increment_cycles(cpu, 10);
}

// Jump if Parity Even
void JPE(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    if (PARITY) cpu->program_counter = address;
    increment_cycles(cpu, 10);
}

// Jump if Parity Odd
void JPO(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    if (!PARITY) cpu->program_counter = address;
    increment_cycles(cpu, 10);
}

// Call
void CALL(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    increment_pc(cpu, 3);
    push_to_stack(cpu, 6);
    cpu->program_counter = address;
    increment_cycles(cpu, 17);
}

// Call if Carry
void CC(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    increment_pc(cpu, 3);
    if (CARRY) {
        push_to_stack(cpu, 6);
        cpu->program_counter = address;
        increment_cycles(cpu, 17);
    }
    else increment_cycles(cpu, 11);
}

// Call if No Carry
void CNC(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    increment_pc(cpu, 3);
    if (!CARRY) {
        push_to_stack(cpu, 6);
        cpu->program_counter = address;
        increment_cycles(cpu, 17);
    }
    else increment_cycles(cpu, 11);
}

// Call if Zero
void CZ(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    increment_pc(cpu, 3);
    if (ZERO) {
        push_to_stack(cpu, 6);
        cpu->program_counter = address;
        increment_cycles(cpu, 17);
    }
    else increment_cycles(cpu, 11);
}

// Call if Not Zero
void CNZ(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    increment_pc(cpu, 3);
    if (!ZERO) {
        push_to_stack(cpu, 6);
        cpu->program_counter = address;
        increment_cycles(cpu, 17);
    }
    else increment_cycles(cpu, 11);
}

// Call if Minus
void CM(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    increment_pc(cpu, 3);
    if (SIGN) {
        push_to_stack(cpu, 6);
        cpu->program_counter = address;
        increment_cycles(cpu, 17);
    }
    else increment_cycles(cpu, 11);
}

// Call if Plus
void CP(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    increment_pc(cpu, 3);
    if (!SIGN) {
        push_to_stack(cpu, 6);
        cpu->program_counter = address;
        increment_cycles(cpu, 17);
    }
    else increment_cycles(cpu, 11);
}

// Call if Parity Even
void CPE(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    increment_pc(cpu, 3);
    if (PARITY) {
        push_to_stack(cpu, 6);
        cpu->program_counter = address;
        increment_cycles(cpu, 17);
    }
    else increment_cycles(cpu, 11);
}

// Call if Parity Odd
void CPO(I8080* cpu) {
    uint16_t address = ((cpu->memory)[cpu->program_counter + 2] << 8) | (cpu->memory)[cpu->program_counter + 1];
    increment_pc(cpu, 3);
    if (!PARITY) {
        push_to_stack(cpu, 6);
        cpu->program_counter = address;
        increment_cycles(cpu, 17);
    }
    else increment_cycles(cpu, 11);
}

// Return
void RET(I8080* cpu) {
    pop_from_stack(cpu, 6);
    increment_cycles(cpu, 10);
}

// Return if Carry
void RC(I8080* cpu) {
    if (CARRY) {
        pop_from_stack(cpu, 6);
        increment_cycles(cpu, 11);
    }
    else {
        increment_cycles(cpu, 5);
        increment_pc(cpu, 1);
    }
}

// Return if Not Carry
void RNC(I8080* cpu) {
    if (!CARRY) {
        pop_from_stack(cpu, 6);
        increment_cycles(cpu, 11);
    }
    else {
        increment_cycles(cpu, 5);
        increment_pc(cpu, 1);
    }
}

// Return if Zero
void RZ(I8080* cpu) {
    if (ZERO) {
        pop_from_stack(cpu, 6);
        increment_cycles(cpu, 11);
    }
    else {
        increment_cycles(cpu, 5);
        increment_pc(cpu, 1);
    }
}

// Return if Not Zero
void RNZ(I8080* cpu) {
    if (!ZERO) {
        pop_from_stack(cpu, 6);
        increment_cycles(cpu, 11);
    }
    else {
        increment_cycles(cpu, 5);
        increment_pc(cpu, 1);
    }
}

// Return if Minus
void RM(I8080* cpu) {
    if (SIGN) {
        pop_from_stack(cpu, 6);
        increment_cycles(cpu, 11);
    }
    else {
        increment_cycles(cpu, 5);
        increment_pc(cpu, 1);
    }
}

// Return if Positive
void RP(I8080* cpu) {
    if (!SIGN) {
        pop_from_stack(cpu, 6);
        increment_cycles(cpu, 11);
    }
    else {
        increment_cycles(cpu, 5);
        increment_pc(cpu, 1);
    }
}

// Return if Parity Even
void RPE(I8080* cpu) {
    if (PARITY) {
        pop_from_stack(cpu, 6);
        increment_cycles(cpu, 11);
    }
    else {
        increment_cycles(cpu, 5);
        increment_pc(cpu, 1);
    }
}

// Return if Parity Odd
void RPO(I8080* cpu) {
    if (!PARITY) {
        pop_from_stack(cpu, 6);
        increment_cycles(cpu, 11);
    }
    else {
        increment_cycles(cpu, 5);
        increment_pc(cpu, 1);
    }
}

// Reset
void RST(I8080* cpu) {
    uint16_t EXP = (cpu->opcode & 0x38);
    increment_pc(cpu, 1);
    push_to_stack(cpu, 6);
    cpu->program_counter = EXP;
    increment_cycles(cpu, 11);
}

// Enable Interrupts
void EI(I8080* cpu) {
    cpu->INTE = 1;
    increment_pc(cpu, 1);
    increment_cycles(cpu, 4);
}

// Disable Interrupts
void DI(I8080* cpu) {
    cpu->INTE = 0;
    increment_pc(cpu, 1);
    increment_cycles(cpu, 4);
}

// Input
void IN(I8080* cpu) {

    uint8_t device = (cpu->memory)[cpu->program_counter+1];

    // read byte of data from device to accumulator

    increment_pc(cpu, 2);
    increment_cycles(cpu, 10);
}

// Output
void OUT(I8080* cpu) {

    uint8_t device = (cpu->memory)[cpu->program_counter+1];

    // write byte of data from accumulator to device

    increment_pc(cpu, 2);
    increment_cycles(cpu, 10);
}

// Halt
void HLT(I8080* cpu) {

    cpu->HALT = 1;

    increment_pc(cpu, 1);
    increment_cycles(cpu, 7);
}

