#include <cstdarg>
#include "systemc.h"

void log(const char * msg, ...);

void run(sc_signal<bool> * clock, int cycles);