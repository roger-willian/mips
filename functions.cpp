#include <cstdarg>
#include <cstdio>
#include "systemc.h"
#include "functions.h"

void log(const char* msg, ...){
#ifdef DEBUG
  va_list list;
  va_start(list, msg);
  vprintf(msg,list);
  va_end(list);
#endif
}

void run(sc_signal<bool> *clock, int cycles){
  int i;
  static int current = 0;
  
  for(i = 0; i < cycles; i++){
      log("\n\n\nCLK: cycle %i\n", current++);
      clock -> write(0);
      sc_start(1, SC_NS);
      clock -> write(1);
      sc_start(1, SC_NS);
    }
}