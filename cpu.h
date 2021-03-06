#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint8_t s:1;   // set if negative
    uint8_t z:1;   // set if zero
    uint8_t p:1;   // set if num bits is even
    uint8_t cy:1;  // set if carry bit
    uint8_t ac:1;  // set if carry bit in 4 bit sequence
    uint8_t pad:3; // extra padding to make 1 byte 
} cpu_flags;

typedef struct {
    cpu_flags flags;
    uint8_t interrupt_enable;
    uint16_t pc;
    uint16_t sp;

    uint64_t cycles;

    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;
} cpu8080;

cpu8080 reset8080();
int disassemble8080(uint8_t *memory, int pc, uint64_t cycles);
void emulate8080(cpu8080* state, uint8_t *memory);

static const uint8_t OPCODES_CYCLES[256] = {
//  0  1   2   3   4   5   6   7   8  9   A   B   C   D   E  F
    4, 10, 7,  5,  5,  5,  7,  4,  4, 10, 7,  5,  5,  5,  7, 4,  // 0
    4, 10, 7,  5,  5,  5,  7,  4,  4, 10, 7,  5,  5,  5,  7, 4,  // 1
    4, 10, 16, 5,  5,  5,  7,  4,  4, 10, 16, 5,  5,  5,  7, 4,  // 2
    4, 10, 13, 5,  10, 10, 10, 4,  4, 10, 13, 5,  5,  5,  7, 4,  // 3
    5, 5,  5,  5,  5,  5,  7,  5,  5, 5,  5,  5,  5,  5,  7, 5,  // 4
    5, 5,  5,  5,  5,  5,  7,  5,  5, 5,  5,  5,  5,  5,  7, 5,  // 5
    5, 5,  5,  5,  5,  5,  7,  5,  5, 5,  5,  5,  5,  5,  7, 5,  // 6
    7, 7,  7,  7,  7,  7,  7,  7,  5, 5,  5,  5,  5,  5,  7, 5,  // 7
    4, 4,  4,  4,  4,  4,  7,  4,  4, 4,  4,  4,  4,  4,  7, 4,  // 8
    4, 4,  4,  4,  4,  4,  7,  4,  4, 4,  4,  4,  4,  4,  7, 4,  // 9
    4, 4,  4,  4,  4,  4,  7,  4,  4, 4,  4,  4,  4,  4,  7, 4,  // A
    4, 4,  4,  4,  4,  4,  7,  4,  4, 4,  4,  4,  4,  4,  7, 4,  // B
    5, 10, 10, 10, 11, 11, 7,  11, 5, 10, 10, 10, 11, 17, 7, 11, // C
    5, 10, 10, 10, 11, 11, 7,  11, 5, 10, 10, 10, 11, 17, 7, 11, // D
    5, 10, 10, 18, 11, 11, 7,  11, 5, 5,  10, 4,  11, 17, 7, 11, // E
    5, 10, 10, 4,  11, 11, 7,  11, 5, 5,  10, 4,  11, 17, 7, 11  // F
};

enum OPCODES{ 
    NOP     = 0x00, 
    
    LXI_B   = 0x01, STAX_B  = 0x02, INX_B   = 0x03, INR_B   = 0x04, DCR_B   = 0x05, MVI_B   = 0x06, RLC     = 0x07, DAD_B   = 0x09, 
    LXI_D   = 0x11, STAX_D  = 0x12, INX_D   = 0x13, INR_D   = 0x14, DCR_D   = 0x15, MVI_D   = 0x16, RAL     = 0x17, DAD_D   = 0x19, 
    LXI_H   = 0x21, SHLD    = 0x22, INX_H   = 0x23, INR_H   = 0x24, DCR_H   = 0x25, MVI_H   = 0x26, DAA     = 0x27, DAD_H   = 0x29, 
    LXI_SP  = 0x31, STA     = 0x32, INX_SP  = 0x33, INR_M   = 0x34, DCR_M   = 0x35, MVI_M   = 0x36, STC     = 0x37, DAD_SP  = 0x39,   
    LDAX_B  = 0x0a, DCX_B   = 0x0b, INR_C   = 0x0c, DCR_C   = 0x0d, MVI_C   = 0x0e, RRC     = 0x0f,
    LDAX_D  = 0x1a, DCX_D   = 0x1b, INR_E   = 0x1c, DCR_E   = 0x1d, MVI_E   = 0x1e, RAR     = 0x1f,
    LHLD    = 0x2a, DCX_H   = 0x2b, INR_L   = 0x2c, DCR_L   = 0x2d, MVI_L   = 0x2e, CMA     = 0x2f,
    LDA     = 0x3a, DCX_SP  = 0x3b, INR_A   = 0x3c, DCR_A   = 0x3d, MVI_A   = 0x3e, CMC     = 0x3f,

    MOV_B_B = 0x40, MOV_B_C = 0x41, MOV_B_D = 0x42, MOV_B_E = 0x43, MOV_B_H = 0x44, MOV_B_L = 0x45, MOV_B_M = 0x46, MOV_B_A = 0x47,
    MOV_D_B = 0x50, MOV_D_C = 0x51, MOV_D_D = 0x52, MOV_D_E = 0x53, MOV_D_H = 0x54, MOV_D_L = 0x55, MOV_D_M = 0x56, MOV_D_A = 0x57,
    MOV_H_B = 0x60, MOV_H_C = 0x61, MOV_H_D = 0x62, MOV_H_E = 0x63, MOV_H_H = 0x64, MOV_H_L = 0x65, MOV_H_M = 0x66, MOV_H_A = 0x67,
    MOV_M_B = 0x70, MOV_M_C = 0x71, MOV_M_D = 0x72, MOV_M_E = 0x73, MOV_M_H = 0x74, MOV_M_L = 0x75, HLT     = 0x76, MOV_M_A = 0x77,
    MOV_C_B = 0x48, MOV_C_C = 0x49, MOV_C_D = 0x4a, MOV_C_E = 0x4b, MOV_C_H = 0x4c, MOV_C_L = 0x4d, MOV_C_M = 0x4e, MOV_C_A = 0x4f,
    MOV_E_B = 0x58, MOV_E_C = 0x59, MOV_E_D = 0x5a, MOV_E_E = 0x5b, MOV_E_H = 0x5c, MOV_E_L = 0x5d, MOV_E_M = 0x5e, MOV_E_A = 0x5f,
    MOV_L_B = 0x68, MOV_L_C = 0x69, MOV_L_D = 0x6a, MOV_L_E = 0x6b, MOV_L_H = 0x6c, MOV_L_L = 0x6d, MOV_L_M = 0x6e, MOV_L_A = 0x6f,
    MOV_A_B = 0x78, MOV_A_C = 0x79, MOV_A_D = 0x7a, MOV_A_E = 0x7b, MOV_A_H = 0x7c, MOV_A_L = 0x7d, MOV_A_M = 0x7e, MOV_A_A = 0x7f,

    ADD_B   = 0x80, ADD_C   = 0x81, ADD_D   = 0x82, ADD_E   = 0x83, ADD_H   = 0x84, ADD_L   = 0x85, ADD_M   = 0x86, ADD_A   = 0x87, 
    SUB_B   = 0x90, SUB_C   = 0x91, SUB_D   = 0x92, SUB_E   = 0x93, SUB_H   = 0x94, SUB_L   = 0x95, SUB_M   = 0x96, SUB_A   = 0x97, 
    ANA_B   = 0xa0, ANA_C   = 0xa1, ANA_D   = 0xa2, ANA_E   = 0xa3, ANA_H   = 0xa4, ANA_L   = 0xa5, ANA_M   = 0xa6, ANA_A   = 0xa7, 
    ORA_B   = 0xb0, ORA_C   = 0xb1, ORA_D   = 0xb2, ORA_E   = 0xb3, ORA_H   = 0xb4, ORA_L   = 0xb5, ORA_M   = 0xb6, ORA_A   = 0xb7,
    ADC_B   = 0x88, ADC_C   = 0x89, ADC_D   = 0x8a, ADC_E   = 0x8b, ADC_H   = 0x8c, ADC_L   = 0x8d, ADC_M   = 0x8e, ADC_A   = 0x8f,
    SBB_B   = 0x98, SBB_C   = 0x99, SBB_D   = 0x9a, SBB_E   = 0x9b, SBB_H   = 0x9c, SBB_L   = 0x9d, SBB_M   = 0x9e, SBB_A   = 0x9f,
    XRA_B   = 0xa8, XRA_C   = 0xa9, XRA_D   = 0xaa, XRA_E   = 0xab, XRA_H   = 0xac, XRA_L   = 0xad, XRA_M   = 0xae, XRA_A   = 0xaf,
    CMP_B   = 0xb8, CMP_C   = 0xb9, CMP_D   = 0xba, CMP_E   = 0xbb, CMP_H   = 0xbc, CMP_L   = 0xbd, CMP_M   = 0xbe, CMP_A   = 0xbf,

    RNZ     = 0xc0, POP_B   = 0xc1, JNZ     = 0xc2, JMP     = 0xc3, CNZ     = 0xc4, PUSH_B  = 0xc5, ADI     = 0xc6, RST_0   = 0xc7, 
    RNC     = 0xd0, POP_D   = 0xd1, JNC     = 0xd2, OUT     = 0xd3, CNC     = 0xd4, PUSH_D  = 0xd5, SUI     = 0xd6, RST_2   = 0xd7, 
    RPO     = 0xe0, POP_H   = 0xe1, JPO     = 0xe2, XTHL    = 0xe3, CPO     = 0xe4, PUSH_H  = 0xe5, ANI     = 0xe6, RST_4   = 0xe7, 
    RP      = 0xf0, POP_PSW = 0xf1, JP      = 0xf2, DI      = 0xf3, CP      = 0xf4, PUSH_PSW= 0xf5, ORI     = 0xf6, RST_6   = 0xf7, 
    
    RZ      = 0xc8, RET     = 0xc9, JZ      = 0xca, /*JMP*    0xcb*/CZ      = 0xcc, CALL    = 0xcd, ACI     = 0xce, RST_1   = 0xcf,
    RC      = 0xd8, /*RET*    0xD9*/JC      = 0xda, IN      = 0xdb, CC      = 0xdc, /*CALL*   0xdd*/SBI     = 0xde, RST_3   = 0xdf,
    RPE     = 0xe8, PCHL    = 0xe9, JPE     = 0xea, XCHG    = 0xeb, CPE     = 0xec, /*CALL*   0xed*/XRI     = 0xee, RST_5   = 0xef,
    RM      = 0xf8, SPHL    = 0xf9, JM      = 0xfa, EI      = 0xfb, CM      = 0xfc, /*CALL*   0xfd*/CPI     = 0xfe, RST_7   = 0xff,
};