#include "systemc.h"
#include "functions.h"
#include "mips.cpp"

int sc_main(int argc, char ** argv){
  
  sc_signal< bool > clock;
  
  // PC signals  
  sc_signal< bool >             PCWriteCond;
  sc_signal< bool >             PCWrite;
  
  // Memory Mux
  sc_signal< bool >             IorD;
  
  // Memory
  sc_signal< bool >             MemRead;
  sc_signal< bool >             MemWrite;
  
  // Data Write Mux
  sc_signal< bool >             MemtoReg;
  
  // Instruction register
  sc_signal< bool >             IRWrite;
  sc_signal< sc_uint<6> >       inst_code;
  
  // Register Bank Write Register
  sc_signal< bool >             RegDst;
  
  // Register file
  sc_signal< bool >             RegWrite;
  
  // ALU signals
  sc_signal< bool >             ALUSrcA;
  sc_signal< sc_uint<2> >       ALUSrcB;
  sc_signal< sc_uint<2> >       ALUOp;
  
  // PC Source signals
  sc_signal< sc_uint<2> >       PCSource;
  
  
  
  mips mcu("MCU");
    mcu.clock(clock);
    mcu.PCWriteCond(PCWriteCond);
    mcu.PCWrite(PCWrite);
    mcu.IorD(IorD);
    mcu.MemRead(MemRead);
    mcu.MemWrite(MemWrite);
    mcu.MemtoReg(MemtoReg);
    mcu.IRWrite(IRWrite);
    mcu.inst_code(inst_code);
    mcu.RegDst(RegDst);
    mcu.RegWrite(RegWrite);
    mcu.ALUSrcA(ALUSrcA);  
    mcu.ALUSrcB(ALUSrcB);
    mcu.ALUOp(ALUOp);
    mcu.PCSource(PCSource);

//   clock.write(0);
//   MemRead.write(0);
//   MemWrite.write(0);
//   address.write(0);
//   IRWrite.write(0);
//   RegWrite.write(0);
//   MemtoReg.write(0);
//   RegDst.write(0);
//   
   sc_start();
//   
//   
//   // read instruction from address #0
//   MemRead.write(1);
//   MemWrite.write(0);
//   address.write(0);
//   IRWrite.write(0);
//   RegWrite.write(0);
//   MemtoReg.write(0);
//   RegDst.write(0);
//   
//   run(&clock, 1);
//   
//   // decode
//   MemRead.write(0);
//   MemWrite.write(0);
//   address.write(0);
//   IRWrite.write(1);
//   RegWrite.write(0);
//   MemtoReg.write(0);
//   RegDst.write(0);
//   
//   run(&clock, 1);
//   
//   // write 3 into register 1
//   MemRead.write(0);
//   MemWrite.write(0);
//   address.write(0);
//   IRWrite.write(0);
//   RegWrite.write(1);
//   MemtoReg.write(0);
//   RegDst.write(1);
//   
//   run(&clock, 1);
//   
//   // read instruction from address #1
//   MemRead.write(1);
//   MemWrite.write(0);
//   address.write(1);
//   IRWrite.write(0);
//   RegWrite.write(0);
//   MemtoReg.write(0);
//   RegDst.write(0);
//   
//   run(&clock, 1);
//   
//   // decode
//   MemRead.write(0);
//   MemWrite.write(0);
//   address.write(0);
//   IRWrite.write(1);
//   RegWrite.write(0);
//   MemtoReg.write(0);
//   RegDst.write(0);
//   
//   run(&clock, 1);
//   
//   // write 2 into register 2
//   MemRead.write(0);
//   MemWrite.write(0);
//   address.write(0);
//   IRWrite.write(0);
//   RegWrite.write(1);
//   MemtoReg.write(0);
//   RegDst.write(1);
//   
//   run(&clock, 1);
//   
//   // read AND R1, R2, R3 from memory address #3
//   MemRead.write(1);
//   MemWrite.write(0);
//   address.write(0x00000003);
//   IRWrite.write(1);
//   RegWrite.write(0);
//   MemtoReg.write(0);
//   RegDst.write(0);
// 
//   run(&clock, 1);
//   
//   MemRead.write(0);
//   
//   run(&clock, 1);
  
//   cout << "instAddress :" << instAddress << endl;
//   cout << "shamt :" << shamt << endl;
//   cout << "funct :" << funct << endl;
//   cout << "inst_code :" << inst_code << endl;
//   cout << "dataA :" << dataA << endl;
//   cout << "dataB :" << dataB << endl;
  
}
