#include "systemc.h"
#include "functions.h"
#include "mips.h"

SC_MODULE (ALU){
  sc_in_clk                     clock;
  sc_in< sc_uint<2> >           ALUOp;
  sc_in< sc_uint<32> >          srcA;
  sc_in< sc_uint<32> >          srcB;
  sc_in< sc_uint<6> >           funct;
  sc_out< sc_uint<32> >         aluResult;
  sc_out< bool >                zero;
  sc_out< sc_uint<32> >         aluOut;
  
  sc_signal< sc_uint<3> >       control;
  
  enum function {
    F_AND = 36,      // 0000
    F_OR = 37,       // 0001
    F_ADD = 32,      // 0010
    F_SUB = 34,      // 0110
    F_LT = 42        // 0111
  };
  
  void action(){
    aluOut.write(aluResult.read());
  }
  
  void aluControl(){
    unsigned int result_ALU;
    
    switch(ALUOp.read()){
      case ADD:
        result_ALU = (srcA.read() + srcB.read());
        break;
      case SUB:
        result_ALU = (srcA.read() - srcB.read());
        break;
      case FUN:
        switch(funct.read()){
          case F_AND:
            result_ALU = (srcA.read() & srcB.read());
            break;
          case F_OR:
            result_ALU = (srcA.read() | srcB.read());
            break;
          case F_ADD:
            result_ALU = (srcA.read() + srcB.read());
            break;
          case F_SUB:
            result_ALU = (srcA.read() - srcB.read());
            break;
          case F_LT:
            if(srcA.read() < srcB.read())
              result_ALU = (0);
            else
              result_ALU = (1);
            break;
          default:
            break;
        }
        break;
      default:
        break;
    }
    
    aluResult.write(result_ALU);
    
    zero.write((result_ALU == 0) ? 1 : 0);
  }
  
  void showInfo(){
    log("ALU: Ent. A: 0x%08x \n", (unsigned int)srcA.read());
    log("ALU: Ent. B: 0x%08x \n", (unsigned int)srcB.read());
    log("ALU: %szero\n", (aluResult.read() == 0) ? "" : "not ");
    switch(ALUOp.read()){
      case ADD:
        log("ALU: adding\n");
        break;
      case SUB:
        log("ALU: subtracting \n");
        break;
      case FUN:
        switch(funct.read()){
          case F_AND:
            log("ALU: anding \n");
          break;
          case F_OR:
            log("ALU: oring \n");
          break;
          case F_ADD:
            log("ALU: adding \n");
          break;
          case F_SUB:
            log("ALU: subtracting \n");
          break;
          case F_LT:
            log("ALU: set if lt \n");
          break;
          default:
            log("ALU: Default \n");
          break;
        }
      default:
        log("ALU: Default \n");
      break;
    }
    log("ALU: Result 0x%08x \n", (unsigned int)aluResult.read());
  }
  
  SC_CTOR(ALU) {
    SC_METHOD(action);
    sensitive << clock.pos();
    
    SC_METHOD(aluControl);
    sensitive << ALUOp;
    sensitive << funct;
    sensitive << srcA;
    sensitive << srcB;
  } 
  
};