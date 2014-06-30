#ifndef MIPS_H
#define MIPS_H
enum operations {
  ADD   = 0,
  SUB   = 1,
  IMM   = 3,
  RTYPE = 2,
  BRANCH = 4,
  NOP = 5
};

enum functions {
  F_SLL = 0,
  F_SRL = 2,
  F_SRA = 3,
  F_SLLV = 4,
  F_SRLV = 6,
  F_SRAV = 7,
  F_JR  = 8,
  F_JALR = 9,
  F_MFHI = 16,
  F_MTHI = 17,
  F_MFLO = 18,
  F_MTLO = 19,
  F_MULT = 24,
  F_MULTU = 25,
  F_DIV = 26,
  F_DIVU = 27,
  F_ADD = 32,
  F_ADDU = 33,
  F_SUB = 34,
  F_SUBU = 35,
  F_AND = 36,
  F_OR  = 37,
  F_XOR = 38,
  F_NOR = 39,
  F_SLT = 42,
  F_SLTU = 43
};

enum formats {
  TYPE_R,
  TYPE_B,
  TYPE_I,
  TYPE_L,
  TYPE_S
};

enum opcodes {
  // R-type = 000000b
  ESPECIAL = 0, // 000000
  // branchs = 000xxxb
  REGIMM= 1,  // 000001
  JUMP  = 2,  // 000010
  JAL   = 3,  // 000011
  BEQ   = 4,  // 000100
  BNE   = 5,  // 000101
  BLEZ  = 6,  // 000110
  BGTZ  = 7,  // 000111
  // immediate = 001xxxb
  ADDI  = 8,  // 001000
  ADDIU = 9,  // 001001
  SLTI  = 10, // 001010
  SLTIU = 11, // 001011
  ANDI  = 12, // 001100
  ORI   = 13, // 001101
  XORI  = 14, // 001110
  LUI   = 15, // 001111
  // load = 100xxxb
  LB    = 32, // 100000
  LW    = 35, // 100011
  LBU   = 36, // 100100
  // store = 101xxxb
  SB    = 40, // 101000
  SW    = 43, // 101011
};

enum controls {
  C_AND = 0,
  C_OR  = 1,
  C_ADD = 2,
  C_SLL = 3,
  C_SRL = 4,
  C_LUI = 5,
  C_SUB = 6,
  C_SLT = 7,
  C_SRAV = 8,
  C_SRLV = 9,
  C_XOR = 10,
  C_SGT = 11,
  C_NOR = 12,
  C_SLLV = 13,
  C_SLTU = 14,
  C_SRA = 15
};

const char* alu_operation[] = {
  "AND",        // 0 
  "OR",         // 1
  "ADD",        // 2
  "SLL",        // 3
  "SRL",        // 4
  "LUI",        // 5
  "SUB",        // 6
  "SLT",        // 7
  "SRAV",       // 8
  "SRLV",       // 9
  "XOR",        // 10
  "SGT",        // 11
  "NOR",        // 12
  "SLLV",       // 13
  "SLTU",       // 14
  "SRA"         // 15
};
#endif
