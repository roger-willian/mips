#include "systemc.h"

SC_MODULE (register32) {
  sc_in_clk             clock;
  sc_in< bool >         enable;
  sc_in< sc_uint<32> >  input;
  sc_out< sc_uint<32> > output;

  void action () {
    if (enable.read() == 1) {
      output.write(input.read());
    }   
  }
  
  void showInfo(const char *name){
    log("%s: content 0x%08x \n", name, (unsigned int)output.read());
  }
  
  SC_CTOR(register32) {
    SC_METHOD(action);
    sensitive << clock.pos();
  }

};