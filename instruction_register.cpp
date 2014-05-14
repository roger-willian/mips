#include "systemc.h"
#include "functions.h"

SC_MODULE (instruction_register) {
  sc_in_clk    clock ;      // Clock input of the design
  sc_in< bool > IRWrite;
  sc_in< sc_uint<32> > instruction; // 32 bit vector
  sc_out< sc_uint<32> > instructionOut; // 32 bit vector
  sc_out< sc_uint<5> > rs; // 5 bit vector 
  sc_out< sc_uint<5> > rt; // 5 bit vector 
  sc_out< sc_uint<5> > rd; // 5 bit vector
  sc_out< sc_uint<16> > address; // 16 bit vector 
  sc_out< sc_uint<5> > shamt; // 5 bit vector 
  sc_out< sc_uint<6> > funct; // 6 bit vector 
  sc_out< sc_uint<6> > inst_code; // 6 bit vector 

  //------------Local Variables Here---------------------
  sc_signal< sc_uint<32> > IR;

  void action () {
    if (IRWrite.read() == 1) {
      log("IR: writing 0x%08x into IR\n", (unsigned int)instruction.read());
      IR = instruction.read();
    }
  }

  void output() {
    rs.write(IR.read()(25,21));
    rt.write(IR.read()(20,16));
    rd.write(IR.read()(15,11));
    shamt.write(IR.read()(10,6));
    funct.write(IR.read()(5,0));
    address.write(IR.read()(15,0));
    inst_code.write(IR.read()(31,26));
    instructionOut.write(IR.read());
  } // End of action

  // Constructor for register file
  SC_CTOR(instruction_register) {
    // cout<<"Executing new"<< name() << endl;
    SC_METHOD(action);
    sensitive << clock.pos();
    SC_METHOD(output);
    sensitive << IR;
  } // End of Constructor

}; // End of Module counter
