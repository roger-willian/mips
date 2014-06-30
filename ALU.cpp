#include "systemc.h"
#include "functions.h"
#include "mips.h"

SC_MODULE (ALU){
  sc_in_clk                     clock;
  sc_in< sc_uint<4> >           ALUControl;
  sc_in< sc_uint<32> >          srcA;
  sc_in< sc_uint<32> >          srcB;
  sc_in< sc_uint<5> >           shamt;
  sc_in< bool >                 InvertZero;
  sc_out< sc_uint<32> >         aluResult;
  sc_out< bool >                zero;
  sc_out< sc_uint<32> >         aluOut;
  
  sc_signal< sc_uint<3> >       control;
  
  
  void action(){
    aluOut.write(aluResult.read());
  }
  
  void aluControl(){
    unsigned int result_ALU;
    
    switch(ALUControl.read()){
      case C_AND:
        result_ALU = (srcA.read() & srcB.read());
        break;
      case C_OR:
        result_ALU = (srcA.read() | srcB.read());
        break;
      case C_ADD:
        result_ALU = (srcA.read() + srcB.read());
        break;
      case C_SUB:
        result_ALU = (srcA.read() - srcB.read());
        break;
      case C_SLT:
        result_ALU = (((int)srcA.read()) < (int)srcB.read()) ? 1 : 0;
        break;
      case C_NOR:
        result_ALU = ~(srcA.read() | srcB.read());
        break;
      case C_SLL:
        result_ALU = srcB.read() << shamt.read();
        break;
      case C_SRL:
        result_ALU = srcB.read() >> shamt.read();
        break;
      case C_LUI:
        result_ALU = (srcB.read()(15,0) << 16);
        break;
      case C_SRAV:
        if(srcB.read()(31,31)) {
          unsigned int inv = ~srcB.read();
          unsigned int shf = inv >> srcA.read();
          result_ALU = ~shf;
        } else {
          unsigned int val = srcB.read();
          result_ALU = val >> srcA.read();
        }
        break;
      case C_SRLV:
        result_ALU = (srcB.read() >> srcA.read());
        break;
      case C_XOR:
        result_ALU = (srcB.read() ^ srcA.read());
        break;
      case C_SGT:
        result_ALU = (((int)srcA.read()) > (int)srcB.read()) ? 1 : 0;
        break;
      case C_SLLV:
        result_ALU = (srcB.read() << srcA.read());
        break;
      case C_SLTU:
        result_ALU = (srcA.read() < srcB.read()) ? 1 : 0;
        break;
      case C_SRA:
        if(srcB.read()(31,31)) {
          unsigned int inv = ~srcB.read();
          unsigned int shf = inv >> shamt.read();
          result_ALU = ~shf;
        } else {
          unsigned int val = srcB.read();
          result_ALU = val >> shamt.read();
        }
        break;
      default:
        break;
    }
    
    aluResult.write(result_ALU);
    
    if(InvertZero.read())
      zero.write((result_ALU == 0) ? 0 : 1);
    else
      zero.write((result_ALU == 0) ? 1 : 0);
  }
  
  void showInfo(){
    log("ALU: Ent. A: 0x%08x \n", (unsigned int)srcA.read());
    log("ALU: Ent. B: 0x%08x \n", (unsigned int)srcB.read());
    log("ALU operation: %s\n", alu_operation[ALUControl.read()]);
    log("ALU: %szero\n", (aluResult.read() == 0) ? "" : "not ");
    log("ALU: Result 0x%08x \n", (unsigned int)aluResult.read());
  }

  SC_CTOR(ALU) {
    SC_METHOD(action);
    sensitive << clock.pos();
    
    SC_METHOD(aluControl);
    sensitive << ALUControl;
    sensitive << shamt;
    sensitive << srcA;
    sensitive << srcB;
    sensitive << InvertZero;
  } 
  
};
