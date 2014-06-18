#include "systemc.h"
#include "functions.h"
#include "mips.h"

#define X 0

enum states {
  PRE_FETCH,
  FETCH,
  DECODE,
  ADDR_COMPUTATION,
  EXECUTION,
  BRANCH_COMPLETION,
  JUMP_COMPLETION,
  MEM_READ_COMPLETION,
  R_COMPLETION,
  MEMORY_ACCESS,
  ADDIU_COMPLETION
};

const char* states_names[]  = {
  "PRE_FETCH",
  "FETCH",
  "DECODE",
  "ADDR_COMPUTATION",
  "EXECUTION",
  "BRANCH_COMPLETION",
  "JUMP_COMPLETION",
  "MEM_READ_COMPLETION",
  "R_COMPLETION",
  "MEMORY_ACCESS",
  "ADDIU_COMPLETION"
};

enum inst_classes {
  R = 0,
  LW = 35,
  SW = 43,
  B = 4,
  J = 2,
  ADDIU = 9
};

SC_MODULE (control_unit) {
  sc_in_clk             clock;
  
  // PC
  sc_out< bool >                 PCWriteCond;    // P1
  sc_out< bool >                 PCWrite;        // P2
  
  // Memory mux
  sc_out< bool >                 IorD;           // P3

  // Memory signals 
  sc_out< bool >                 MemRead;        // P4
  sc_out< bool >                 MemWrite;       // P5
  
  // Instruction register signals 
  sc_out< bool >                 IRWrite;        // P6
  sc_in< sc_uint<32> >            inst_code;      // P7
  
  // Register Bank Write Register Mux
  sc_out< bool >                 MemtoReg;       // P8: Memory or ALU
  
  // Register Bank Write Data Mux
  sc_out< bool >                 RegDst;         // P9: rd or rt
  
  // Register file signals
  sc_out< bool >                 RegWrite;       // P10: Enable writing
  
  // ALUSrcA Mux signals
  sc_out< bool >                 ALUSrcA;        // P11
  
  // ALUSrcB Mux signals
  sc_out< sc_uint<2> >           ALUSrcB;        // P12
  
  // ALU signals
  sc_out< sc_uint<6> >           ALUOp;          // P13
  
  // PC Source Mux signals
  sc_out< sc_uint<2> >           PCSource;       // P14
  
  sc_in< bool >                  reset;         // P15
  
  
  sc_signal< sc_uint<8> >         curr_st;
  sc_signal< sc_uint<8> >         next_st;

  void set_signals(bool pcWriteCond,
                   bool pcWrite,
                   bool iord,
                   bool memRead,
                   bool memWrite,
                   bool memToReg,
                   bool irWrite,
                   bool regDst,
                   bool regWrite,
                   bool aluSrcA,
                   sc_uint<2> aluSrcB,
                   sc_uint<2> aluOp,
                   sc_uint<2> pcSrc){
    PCWriteCond.write(pcWriteCond);
    PCWrite.write(pcWrite);
    IorD.write(iord);
    MemRead.write(memRead);
    MemWrite.write(memWrite);
    MemtoReg.write(memToReg);
    IRWrite.write(irWrite);
    RegDst.write(regDst);
    RegWrite.write(regWrite);
    ALUSrcA.write(aluSrcA);
    ALUSrcB.write(aluSrcB);
    ALUOp.write(aluOp);
    PCSource.write(pcSrc);
  }
  
  void action (){
    curr_st.write(next_st.read());
  }
  
  void logic () {
    
    switch (curr_st.read()){
      case PRE_FETCH:
        //
        set_signals(0,0,0,1,0,X,1,X,0,0,1,NOP,X);
        next_st.write(FETCH);
        break;
      case FETCH:
        // IR <- MEM[PC]
        // AluOut <- PC + 4
        set_signals(0,0,0,1,0,X,1,X,0,0,1,ADD,X);
        next_st.write(DECODE);
        break;
      case DECODE:
        // A <= Reg[rs]
        // B <= Reg[rt]
        // PC <= AluOut
        // AluOut <= PC + (sign-extend(addr << 2))
        set_signals(X,1,X,0,0,X,0,X,0,0,3,ADD,1);
        switch (inst_code.read()(31,26)){
          case R:
            next_st.write(EXECUTION);
            break;
          case LW:
          case SW:
          case ADDIU:
            next_st.write(ADDR_COMPUTATION);
            break;
          case B:
            next_st.write(BRANCH_COMPLETION);
            break;
          case J:
            next_st.write(JUMP_COMPLETION);
            break;
          default:
            next_st.write(FETCH);
            break;
        }
        break;
      case EXECUTION:
        // AluOut <= A op B
        set_signals(0,0,X,X,0,X,0,X,0,1,0,inst_code.read()(5,0),X);
        next_st.write(R_COMPLETION);
        break;
      case R_COMPLETION:
        // Reg[rd] <= AluOut
        set_signals(0,0,X,X,0,0,0,1,1,X,X,NOP,X);
        next_st.write(FETCH);
        break;
      case ADDR_COMPUTATION:
        // AluOut <= A + sign-extend(addr)
        set_signals(0,0,X,X,0,X,0,X,0,1,2,ADD,X);
        if(inst_code.read()(31,26) == ADDIU)
          next_st.write(ADDIU_COMPLETION);
        else
          next_st.write(MEMORY_ACCESS);
        break;
      case ADDIU_COMPLETION:
        // Reg[rt] <= AluOut
        set_signals(0,0,X,X,0,0,0,0,1,X,X,NOP,X);
        next_st.write(FETCH);
        break;
      case MEMORY_ACCESS:
        // Load: MDR <= Memory[ALUOut] OR
        // Store: Memory[ALUOut] <= B
        if (inst_code.read()(31,26) == LW) {
          set_signals(0,0,1,1,0,X,0,X,0,X,X,NOP,X);
          next_st.write(MEM_READ_COMPLETION);
        } else {
          set_signals(0,0,1,X,1,X,0,X,0,X,X,NOP,X);
          next_st.write(FETCH);
        }
        break;
      case MEM_READ_COMPLETION:
        // Reg[rt] <= MDR
        set_signals(0,0,X,X,0,1,0,0,1,X,X,NOP,X);
        next_st.write(FETCH);
        break;
      case BRANCH_COMPLETION:
        // if(A == B) PC <= AluOut
        set_signals(1,0,X,X,0,X,0,X,0,1,0,SUB,1);
        next_st.write(FETCH);
        break;
      case JUMP_COMPLETION:
        // PC <= {PC[31,28],IR[25,0]<<2)}
        set_signals(X,1,X,X,0,X,0,X,0,X,X,NOP,2);
        next_st.write(FETCH);
        break;
      default:
        log("CONTROL invalid %d\n", (unsigned int)inst_code.read()(31,26));
        return;
    }
    if(reset.read() == 1) {
      next_st.write(PRE_FETCH);
      return;
    }
  }
  
  void showInfo(){
    log("CONTROL: inst_code 0x%08x \n", (unsigned int)inst_code.read());
    log("CONTROL: curr_st %s \n", states_names[curr_st.read()]);
    log("CONTROL: next_st %s \n", states_names[next_st.read()]);
  }

  SC_CTOR(control_unit) {
    SC_METHOD(action);
    sensitive << clock.pos();
    SC_METHOD(logic);
    sensitive << inst_code;
    sensitive << reset;
    sensitive << curr_st;
  }

};
