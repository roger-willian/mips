#include "systemc.h"
#include "mips.h"

SC_MODULE (alu_control) {
  
  sc_in< sc_uint<6> > OpCode;
  sc_in< sc_uint<6> > funct;
  sc_in< sc_uint<3> > ALUOp;
  sc_out< sc_uint<4> > ALUControl;
  sc_out< bool > InvertZero;
  

  void action () {
    InvertZero.write(false);
    switch(ALUOp.read()){
      case ADD:
        ALUControl.write(C_ADD);
        break;
      case SUB:
        ALUControl.write(C_SUB);
        break;
      case IMM:
        switch(OpCode.read()){
          case ADDI:
          case ADDIU:
            ALUControl.write(C_ADD);
            break;
          case SLTI:
            ALUControl.write(C_SLT);
            break;
          case SLTIU:
            ALUControl.write(C_SLTU);
            break;
          case ANDI:
            ALUControl.write(C_AND);
            break;
          case ORI:
            ALUControl.write(C_OR);
            break;
          case XORI:
            ALUControl.write(C_XOR);
            break;
          case LUI:
            ALUControl.write(C_LUI);
            break;
          default:
            break;
        }
        break;
      case RTYPE:
        switch(funct.read()){
          case F_SLL:
            ALUControl.write(C_SLL);
            break;            
          case F_SRL:
            ALUControl.write(C_SRL);
            break;
          case F_SRA:
            ALUControl.write(C_SRA);
            break;
          case F_SLLV:
            ALUControl.write(C_SLLV);
            break;            
          case F_SRLV:
            ALUControl.write(C_SRLV);
            break;
          case F_SRAV:
            ALUControl.write(C_SRAV);
            break;
                
          case F_MFHI:
          case F_MTHI:
          case F_MFLO:
          case F_MTLO:
          case F_MULT:
          case F_MULTU:
          case F_DIV:
          case F_DIVU:
            // Essas funções ainda não foram implementadas
            break;
            
          case F_ADD:
          case F_ADDU:
            ALUControl.write(C_ADD);
            break;          
          case F_SUB:
          case F_SUBU:
            ALUControl.write(C_SUB);
            break;
          case F_AND:
            ALUControl.write(C_AND);
            break;          
          case F_OR:
            ALUControl.write(C_OR);
            break;            
          case F_XOR:
            ALUControl.write(C_XOR);
            break;
          case F_NOR:            
            ALUControl.write(C_NOR);
            break;          
          case F_SLT:
            ALUControl.write(C_SLT);
            break;            
          case F_SLTU:
            ALUControl.write(C_SLTU);
            break;
          default:
            break;
        }
        break;
      case BRANCH:
        switch(OpCode.read()){
          case BNE:
            InvertZero.write(true);
          case BEQ:
            ALUControl.write(C_SUB);            
            break;
          case BGTZ:
            InvertZero.write(true);
          case BLEZ:
            ALUControl.write(C_SGT);
            break;
          default:
            break;
        }
        break;
      case NOP:
      default:
        break;
    }
  }
  
  SC_CTOR(alu_control) {
    SC_METHOD(action);
    sensitive << ALUControl;
    sensitive << OpCode;
    sensitive << funct;
    sensitive << ALUOp;
  }
};
