#include <stdint.h>

typedef struct {

    // Registers

    uint8_t wz[2]; // WZ -> 01
    uint8_t registers[8]; // BCDEHLAF -> 01234567
    uint16_t program_counter;
    uint16_t stack_pointer;
    uint8_t opcode;

    // ALU

    //uint8_t accumulator;
    uint8_t temp_accumulator;
    //uint8_t flags; // zero, carry, sign, parity, aux carry -> 01234. Also register A
    uint8_t TMP;

    uint8_t memory[65536];

    uint64_t cycles;
    

} I8080;

void push_to_stack(I8080* cpu, uint8_t register);
void pop_from_stack(I8080* cpu, uint8_t register);
void increment_cycles(I8080* cpu, uint8_t num);
void cycle(I8080* cpu);
void init_cpu(I8080* cpu);

// CARRY BIT INSTRUCTIONS

void STC(I8080* cpu);
void CMC(I8080* cpu);

// SINGLE REGISTER INSTRUCTIONS

void INR(I8080* cpu);
void DCR(I8080* cpu);
void CMA(I8080* cpu);
void DAA(I8080* cpu);

// NOP INSTRUCTION

void NOP(I8080* cpu);

// DATA TRANSFER INSTRUCTIONS

void MOV(I8080* cpu);
void STAX(I8080* cpu);
void LDAX(I8080* cpu);

// REGISTER OR MEMORY TO ACCUMULATOR INSTRUCTIONS

void ADD(I8080* cpu);
void ADC(I8080* cpu);
void SUB(I8080* cpu);
void SBB(I8080* cpu);
void ANA(I8080* cpu);
void XRA(I8080* cpu);
void ORA(I8080* cpu);
void CMP(I8080* cpu);

// ROTATE ACCUMULATOR INSTRUCTIONS

void RLC(I8080* cpu);
void RRC(I8080* cpu);
void RAL(I8080* cpu);
void RAR(I8080* cpu);

// REGISTER PAIR INSTRUCTIONS

void PUSH(I8080* cpu);
void POP(I8080* cpu);
void DAD(I8080* cpu);
void INX(I8080* cpu);
void DCX(I8080* cpu);

void XCHG(I8080* cpu);
void XTHL(I8080* cpu);
void SPHL(I8080* cpu);

// IMMEDIATE INSTRUCTIONS

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

// DIRECT ADDRESSING INSTRUCTIONS

void STA(I8080* cpu);
void LDA(I8080* cpu);
void SHLD(I8080* cpu);
void LHLD(I8080* cpu);

// JUMP INSTRUCTIONS

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

// CALL INSTRUCTIONS

void CALL(I8080* cpu);
void CC(I8080* cpu);
void CNC(I8080* cpu);
void CZ(I8080* cpu);
void CNZ(I8080* cpu);
void CP(I8080* cpu);
void CM(I8080* cpu);
void CPE(I8080* cpu);
void CPO(I8080* cpu);

// RETURN INSTRUCTIONS

void RET(I8080* cpu);
void RC(I8080* cpu);
void RNC(I8080* cpu);
void RZ(I8080* cpu);
void RNZ(I8080* cpu);
void RM(I8080* cpu);
void RP(I8080* cpu);
void RPE(I8080* cpu);
void RPO(I8080* cpu);

// RST INSTRUCTION

void RST(I8080* cpu);

// INTERRUPT FLIP-FLOP INSTRUCTIONS

void EI(I8080* cpu);
void DI(I8080* cpu);

// INPUT/OUTPUT INSTRUCTIONS;

void IN(I8080* cpu);
void OUT(I8080* cpu);

// HLT INSTRUCTION

void HLT(I8080* cpu);