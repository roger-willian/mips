#include "systemc.h"

SC_MODULE (register_file) {
  sc_in_clk    clock ;      // Clock input of the design
  sc_in< bool >  RegWrite;
  sc_in< sc_uint<5> > addrA; // 5 bit vector 
  sc_in< sc_uint<5> > addrB; // 5 bit vector 
  sc_in< sc_uint<5> > addrWrite; // 5 bit vector 
  sc_in< sc_uint<32> > dataWrite; // 32 bit vector 
  sc_out< sc_uint<32> > dataA; // 32 bit vector 
  sc_out< sc_uint<32> > dataB; // 32 bit vector 

  //------------Local Variables Here---------------------
  sc_uint<32> bank[32];

  //------------Code Starts Here-------------------------
  // Below function implements actual counter logic
  void action () {
    if (RegWrite.read() == 1) {
      if (addrWrite.read() != 0){
#ifdef DEBUG
        printf("REG_BANK: Writing 0x%08x into register #%i\n", (unsigned int)dataWrite.read(), (unsigned short)addrWrite.read());
#endif
        bank[addrWrite.read()] = dataWrite.read();
      }
    }
#ifdef DEBUG
        puts("REG_BANK: outputs");
        printf("\tA: 0x%08x\n", (unsigned int)bank[addrA.read()]);
        printf("\tB: 0x%08x\n", (unsigned int)bank[addrB.read()]);
#endif
    dataA.write(bank[addrA.read()]);
    dataB.write(bank[addrB.read()]);

  } // End of action

  // Constructor for register file
  SC_CTOR(register_file) {
    // cout<<"Executing new"<< name() << endl;
    SC_METHOD(action);
    sensitive << clock.pos();
    bank[0] = 0;
  } // End of Constructor

}; // End of Module counter