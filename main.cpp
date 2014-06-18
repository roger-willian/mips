#include "systemc.h"
#include "functions.h"
#include "operational_unit.cpp"
#include "control_unit.cpp"

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
  sc_signal< sc_uint<32> >      inst_code;
  
  // Register Bank Write Register
  sc_signal< bool >             RegDst;
  
  // Register file
  sc_signal< bool >             RegWrite;
  
  // ALU signals
  sc_signal< bool >             ALUSrcA;
  sc_signal< sc_uint<2> >       ALUSrcB;
  sc_signal< sc_uint<6> >       ALUOp;
  
  // PC Source signals
  sc_signal< sc_uint<2> >       PCSource;
  
  //reset
  sc_signal< bool >             reset;
  
  int i;
  char opt;
  
  operational_unit ou("OU");
    ou.clock(clock);
    ou.ALUOp(ALUOp);
    ou.PCWriteCond(PCWriteCond);
    ou.PCWrite(PCWrite);
    ou.IorD(IorD);
    ou.MemRead(MemRead);
    ou.MemWrite(MemWrite);
    ou.MemtoReg(MemtoReg);
    ou.IRWrite(IRWrite);
    ou.irOut(inst_code);
    ou.RegDst(RegDst);
    ou.RegWrite(RegWrite);
    ou.ALUSrcA(ALUSrcA);  
    ou.ALUSrcB(ALUSrcB);
    ou.PCSource(PCSource);
    
  control_unit control("CONTROL");
    control.clock(clock);
    control.PCWriteCond(PCWriteCond);
    control.PCWrite(PCWrite);
    control.IorD(IorD);
    control.MemRead(MemRead);
    control.MemWrite(MemWrite);
    control.MemtoReg(MemtoReg);
    control.IRWrite(IRWrite);
    control.inst_code(inst_code);
    control.RegDst(RegDst);
    control.RegWrite(RegWrite);
    control.ALUSrcA(ALUSrcA);  
    control.ALUSrcB(ALUSrcB);
    control.ALUOp(ALUOp);
    control.PCSource(PCSource);
    control.reset(reset);

//   clock.write(0);
//   MemRead.write(0);
//   MemWrite.write(0);
//   address.write(0);
//   IRWrite.write(0);
//   RegWrite.write(0);
//   MemtoReg.write(0);
//   RegDst.write(0);
// 
   reset.write(1);
   sc_start();
   reset.write(0);
   ou.dumpMemory();
   while((opt = getchar()) != 's'){
      cout << "<<<<<<" << endl;
      run(&clock, 1);
      cout << ">>>>>>" << endl;
      ou.showInfo();
      control.showInfo();
   }   
   ou.dumpMemory();
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
