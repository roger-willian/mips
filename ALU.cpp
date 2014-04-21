#include "systemc.h"
#include "functions.h"

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
  
  enum operation {
    ADD,
    SUB,
    FUNCT
  };
  
  enum function {
    MUL,
    DIV
  };
  
  void action(){
    aluOut.write(aluResult.read());
  }
  
  void aluControl(){
    switch(ALUOp.read()){
      case ADD:
        aluResult.write(srcA.read() + srcB.read());
        log("ALU: adding A: 0x%08x B: 0x%08x ", (unsigned int)srcA.read(), (unsigned int)srcB.read());
        break;
      case SUB:
        aluResult.write(srcA.read() - srcB.read());
        log("ALU: subtracting A: 0x%08x B: 0x%08x ", (unsigned int)srcA.read(), (unsigned int)srcB.read());
        break;
      case FUNCT:
        switch(funct.read()){
          case MUL:
        aluResult.write(srcA.read() * srcB.read());
            log("ALU: multiplying A: 0x%08x B: 0x%08x ", (unsigned int)srcA.read(), (unsigned int)srcB.read());
            break;
          case DIV:
        aluResult.write(srcA.read() / srcB.read());
            log("ALU: dividing A: 0x%08x B: 0x%08x ", (unsigned int)srcA.read(), (unsigned int)srcB.read());
            break;
          default:
            break;
        }
        break;
      default:
        break;
    }
    zero.write((aluResult.read() == 0) ? 1 : 0);
    log("ALU: %szero\n", (zero.read() == 0) ? "" : "not ");
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