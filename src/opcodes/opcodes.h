#ifndef OPCODES_H
#define OPCODES_H

// Every instruction the CPU implements, grouped by family. Each family lives
// in the .c file named alongside it. I8080.c wires these into the dispatch
// table; the implementations themselves also pull in util.h.

#include "I8080.h"

// CARRY BIT INSTRUCTIONS -> carry_bit.c

void STC(I8080* cpu);
void CMC(I8080* cpu);

// SINGLE REGISTER INSTRUCTIONS -> single_register.c

void INR(I8080* cpu);
void DCR(I8080* cpu);
void CMA(I8080* cpu);
void DAA(I8080* cpu);

// NOP INSTRUCTION -> nop.c

void NOP(I8080* cpu);

// DATA TRANSFER INSTRUCTIONS -> data_transfer.c

void MOV(I8080* cpu);
void STAX(I8080* cpu);
void LDAX(I8080* cpu);

// REGISTER OR MEMORY TO ACCUMULATOR INSTRUCTIONS -> accumulator.c

void ADD(I8080* cpu);
void ADC(I8080* cpu);
void SUB(I8080* cpu);
void SBB(I8080* cpu);
void ANA(I8080* cpu);
void XRA(I8080* cpu);
void ORA(I8080* cpu);
void CMP(I8080* cpu);

// ROTATE ACCUMULATOR INSTRUCTIONS -> rotate_accumulator.c

void RLC(I8080* cpu);
void RRC(I8080* cpu);
void RAL(I8080* cpu);
void RAR(I8080* cpu);

// REGISTER PAIR INSTRUCTIONS -> register_pair.c

void PUSH(I8080* cpu);
void POP(I8080* cpu);
void DAD(I8080* cpu);
void INX(I8080* cpu);
void DCX(I8080* cpu);

void XCHG(I8080* cpu);
void XTHL(I8080* cpu);
void SPHL(I8080* cpu);

// IMMEDIATE INSTRUCTIONS -> immediate.c

void LXI(I8080* cpu);
void MVI(I8080* cpu);
void ADI(I8080* cpu);
void ACI(I8080* cpu);
void SUI(I8080* cpu);
void SBI(I8080* cpu);
void ANI(I8080* cpu);
void XRI(I8080* cpu);
void ORI(I8080* cpu);
void CPI(I8080* cpu);

// DIRECT ADDRESSING INSTRUCTIONS -> direct_addressing.c

void STA(I8080* cpu);
void LDA(I8080* cpu);
void SHLD(I8080* cpu);
void LHLD(I8080* cpu);

// JUMP INSTRUCTIONS -> jump.c

void PCHL(I8080* cpu);
void JMP(I8080* cpu);
void JC(I8080* cpu);
void JNC(I8080* cpu);
void JZ(I8080* cpu);
void JNZ(I8080* cpu);
void JP(I8080* cpu);
void JM(I8080* cpu);
void JPE(I8080* cpu);
void JPO(I8080* cpu);

// CALL INSTRUCTIONS -> call.c

void CALL(I8080* cpu);
void CC(I8080* cpu);
void CNC(I8080* cpu);
void CZ(I8080* cpu);
void CNZ(I8080* cpu);
void CP(I8080* cpu);
void CM(I8080* cpu);
void CPE(I8080* cpu);
void CPO(I8080* cpu);

// RETURN INSTRUCTIONS -> return.c

void RET(I8080* cpu);
void RC(I8080* cpu);
void RNC(I8080* cpu);
void RZ(I8080* cpu);
void RNZ(I8080* cpu);
void RM(I8080* cpu);
void RP(I8080* cpu);
void RPE(I8080* cpu);
void RPO(I8080* cpu);

// RST INSTRUCTION -> rst.c

void RST(I8080* cpu);

// INTERRUPT FLIP-FLOP INSTRUCTIONS -> interrupt.c

void EI(I8080* cpu);
void DI(I8080* cpu);

// INPUT/OUTPUT INSTRUCTIONS -> io.c

void IN(I8080* cpu);
void OUT(I8080* cpu);

// HLT INSTRUCTION -> hlt.c

void HLT(I8080* cpu);

#endif
