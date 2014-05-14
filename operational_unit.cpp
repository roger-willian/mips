#include "systemc.h"
#include "functions.h"
#include "instruction_register.cpp"
#include "memory.cpp"
#include "register_file.cpp"
#include "register32.cpp"
#include "ALU.cpp"

SC_MODULE(operational_unit){
  
  sc_in_clk                     clock ;         // P0: Clock input of the design
  
  // PC
  sc_in< bool >                 PCWriteCond;    // P1
  sc_in< bool >                 PCWrite;        // P2
  sc_signal< sc_uint<32> >      pcOut;
  sc_signal< sc_uint<32> >      pcIn;
  sc_signal< bool >             enablePC;
  
  // Memory mux
  sc_in< bool >                 IorD;           // P3

  // Memory signals 
  sc_in< bool >                 MemRead;        // P4
  sc_in< bool >                 MemWrite;       // P5
  sc_signal< sc_uint<32> >      address;
  sc_signal< sc_uint<32> >      memData;        // data read from memory  
  
  // Instruction register signals 
  sc_in< bool >                 IRWrite;        // P6
  sc_out< sc_uint<6> >          inst_code;      // P7
  sc_signal< sc_uint<16> >      instAddress;
  sc_signal< sc_uint<5> >       shamt;
  sc_signal< sc_uint<6> >       funct;
  sc_signal< sc_uint<5> >       rs;             // Source register
  sc_signal< sc_uint<5> >       rt;             // Target register
  sc_signal< sc_uint<5> >       rd;             // Destination register
  sc_signal< sc_uint<32> >      irOut;          // instruction within IR
  
  // Register Bank Write Register Mux
  sc_in< bool >                 MemtoReg;       // P8: Memory or ALU
  
  // Register Bank Write Data Mux
  sc_in< bool >                 RegDst;         // P9: rd or rt
  
  // Register file signals
  sc_in< bool >                 RegWrite;       // P10: Enable writing
  sc_signal< sc_uint<32> >      dataA;
  sc_signal< sc_uint<32> >      dataB;
  sc_signal< sc_uint<5> >       addrWrite;
  sc_signal< sc_uint<5> >       addrA;          // address register A
  sc_signal< sc_uint<5> >       addrB;          // address register B
  sc_signal< sc_uint<32> >      dataWrite;      // data to write
  
  // Memory data register signals
  sc_signal< sc_uint<32> >      memoryDataRegisterOut;  // To mux
  sc_signal< bool >             memoryDataRegisterEnable;
  
  // ALUSrcA Mux signals
  sc_in< bool >                 ALUSrcA;        // P11
  
  // ALUSrcB Mux signals
  sc_in< sc_uint<2> >           ALUSrcB;        // P12
  
  // ALU signals
  sc_in< sc_uint<2> >           ALUOp;          // P13
  sc_signal< bool >             zero;
  sc_signal< sc_uint<32> >      aluOut;         // ALU output
  sc_signal< sc_uint<32> >      srcA;
  sc_signal< sc_uint<32> >      srcB;
  sc_signal< sc_uint<32> >      aluResult;  
  
  // PC Source Mux signals
  sc_in< sc_uint<2> >           PCSource;       // P14
  
  instruction_register * ir;
  memory * mem;
  register_file * rf;
  register32 * memoryDataRegister;
  ALU * alu;
  register32 * pc;
  
  void dumpMemory(){
    mem->dumpMem();
  }  
  
  void muxDataWrite(){
    if (MemtoReg.read() == 1){
      dataWrite.write(memoryDataRegisterOut.read());
    }
    else{
      dataWrite.write(aluOut.read());
    }
  }
  
  void muxWriteRegister(){
    if(RegDst.read() == 1){
      addrWrite.write(rd.read());
    }
    else{
      addrWrite.write(rt.read());
    }
  }
  
  void muxAluSrcA(){
    if(ALUSrcA.read() == 1){
       srcA.write(dataA.read());
    }
    else{
      srcA.write(pcOut.read());
    }
  }
  
  void muxAluSrcB(){
    switch(ALUSrcB.read()){
      case 0:
        srcB.write(dataB.read());
        break;
      case 1:
        srcB.write(4);
        break;
      case 2:
        // sign extend
        if(instAddress.read()(15,15))
          srcB.write(0xffff0000 | (unsigned int)instAddress.read());
        else
          srcB.write((unsigned int)instAddress.read());
        break;
      default:
        // sign extend with 2 shifts left
        if(instAddress.read()(15,15))
          srcB.write((0xffff0000 | (unsigned int)instAddress.read()) << 2);
        else
          srcB.write(((unsigned int)instAddress.read()) << 2);
        break;
    }
  }
  
  void muxPC(){
    switch(PCSource.read()){
      case 0:
        pcIn.write(aluResult.read());
        break;
      case 1:
        pcIn.write(aluOut.read());
        break;
      case 2:
        // instruction(25,0)
        pcIn.write((((unsigned int)irOut.read()(25,0)) << 2) | (((unsigned int)pcOut.read()(31,28)) << 28));
        break;
      default:
        log("ERROR: MUX_PC invalid control signal");
        break;
    }
  }
  
  void enablePCLogic(){
    if((zero.read() && PCWriteCond.read()) || PCWrite.read() ){
      enablePC.write(1);
    }
    else{
      enablePC.write(0); 
    }
  }
  
  void muxMemory(){
    if(IorD.read() == 1){
      address.write(aluOut.read() & 0x0000FFFF); 
    }
    else{
       address.write(pcOut.read() & 0x0000FFFF); 
    }
  }
  
  void showInfo(){
    alu -> showInfo();
    pc -> showInfo("PC");
    
    log("OU: pcOut              0x%08x\n", (unsigned int)pcOut.read());
    log("OU: pcIn               0x%08x\n", (unsigned int)pcIn.read());
    log("OU: enablePC           %d\n", (bool)enablePC.read());
    log("OU: address            0x%08x\n", (unsigned int)address.read());
    log("OU: memData            0x%08x\n", (unsigned int)memData.read());
    log("OU: instAddress        0x%04x\n", (unsigned int)instAddress.read());
    log("OU: shamt              %d\n", (unsigned int)shamt.read());
    log("OU: funct              %d\n", (unsigned int)funct.read());
    log("OU: rs                 %d\n", (unsigned int)rs.read());
    log("OU: rt                 %d\n", (unsigned int)rt.read());
    log("OU: rd                 %d\n", (unsigned int)rd.read());
    log("OU: irOut              0x%08x\n", (unsigned int)irOut.read());
    log("OU: dataA              0x%08x\n", (unsigned int)dataA.read());
    log("OU: dataB              0x%08x\n", (unsigned int)dataB.read());
    log("OU: addrWrite          %d\n", (unsigned int)addrWrite.read());
    log("OU: addrA              %d\n", (unsigned int)addrA.read());
    log("OU: addrB              %d\n", (unsigned int)addrB.read());
    log("OU: dataWrite          0x%08x\n", (unsigned int)dataWrite.read());
    log("OU: memoryDataRegisterOut 0x%08x\n", (unsigned int)memoryDataRegisterOut.read());
    log("OU: memoryDataRegisterEnable %d\n", (bool)memoryDataRegisterEnable.read());
    log("OU: zero               %d\n", (bool)zero.read());
    log("OU: aluOut             0x%08x\n", (unsigned int)aluOut.read());
    log("OU: srcA               0x%08x\n", (unsigned int)srcA.read());
    log("OU: srcB               0x%08x\n", (unsigned int)srcB.read());
    log("OU: aluResult          0x%08x\n", (unsigned int)aluResult.read());
  }
  
  SC_CTOR(operational_unit) {
    ir = new instruction_register("IR");
      ir->clock(clock);
      ir->IRWrite(IRWrite);
      ir->instruction(memData);
      ir->rs(rs);
      ir->rt(rt);
      ir->rd(rd);
      ir->address(instAddress);
      ir->shamt(shamt);
      ir->funct(funct);
      ir->inst_code(inst_code);
      ir->instructionOut(irOut);
    
    mem = new memory("MEM");
      mem->clock(clock);
      mem->MemRead(MemRead);
      mem->MemWrite(MemWrite);
      mem->address(address);
      mem->writeData(dataB);
      mem->memData(memData);
      
    rf = new register_file("RF");
      rf->clock(clock);
      rf->RegWrite(RegWrite);
      rf->addrA(rs);
      rf->addrB(rt);
      rf->addrWrite(addrWrite);
      rf->dataWrite(dataWrite);
      rf->dataA(dataA);
      rf->dataB(dataB);
      
    memoryDataRegister = new register32("MEMORY_DATA_REGISTER");
      memoryDataRegister->clock(clock);
      memoryDataRegister->enable(memoryDataRegisterEnable);
      memoryDataRegister->input(memData);
      memoryDataRegister->output(memoryDataRegisterOut);
    
    alu = new ALU("ALU");
      alu->clock(clock);
      alu->aluResult(aluResult);
      alu->zero(zero);
      alu->aluOut(aluOut);
      alu->srcA(srcA);
      alu->srcB(srcB);
      alu->ALUOp(ALUOp);
      alu->funct(funct);
      
   pc = new register32("PC");
      pc->clock(clock);
      pc->enable(enablePC);
      pc->input(pcIn);
      pc->output(pcOut);
    
    SC_METHOD(muxDataWrite){
      sensitive << MemtoReg;
      sensitive << memoryDataRegisterOut;
      sensitive << aluOut;
    }
    
    SC_METHOD(muxWriteRegister){
      sensitive << RegDst;
      sensitive << rd;
      sensitive << rt;
    }
    
    SC_METHOD(muxAluSrcA){
      sensitive << ALUSrcA;
      sensitive << pcOut;
      sensitive << dataA;
    }
    
    SC_METHOD(muxAluSrcB){
      sensitive << ALUSrcB;
      sensitive << dataB;
      sensitive << instAddress;
    }
    
    SC_METHOD(muxPC){
      sensitive << PCSource;
      sensitive << aluResult;
      sensitive << aluOut;
      sensitive << irOut;
    }
    
    SC_METHOD(enablePCLogic){
      sensitive << zero;
      sensitive << PCWrite;
      sensitive << PCWriteCond;
    }
    
    SC_METHOD(muxMemory){
      sensitive << IorD;
      sensitive << aluOut;
      sensitive << pcOut;
    }
    
    memoryDataRegisterEnable.write(1);
  }

};
