#include "systemc.h"
#include <stdio.h>

#ifndef MEM_SIZE
#define MEM_SIZE 32
#endif

SC_MODULE(memory){
  sc_in_clk clock;
  sc_in< bool >         MemRead;
  sc_in< bool >         MemWrite;
  sc_in< sc_uint<32> >  address;
  sc_in< sc_uint<32> >  writeData;
  sc_out< sc_uint<32> > memData;
  
  sc_uint<32>           regMem[MEM_SIZE];
  
  void action(){
    
    if (MemWrite == 1){
#ifdef DEBUG
      printf("MEM: writing 0x%08x into address 0x%08x\n", (unsigned int)writeData.read(), (unsigned int)address.read());
#endif
      regMem[address.read()(5,0)] = writeData.read();
    }
    if (MemRead == 1){
#ifdef DEBUG
      printf("MEM: reading 0x%08x from address 0x%08x\n", (unsigned int)regMem[address.read()(5,0)], (unsigned int)address.read());
#endif
      memData.write(regMem[address.read()(5,0)]);
    }    
  }
  
  void dumpMem(){
    int i = 0;
    puts("Memory contents:");
    for(i=0; i<MEM_SIZE; i++){
      printf("[0x%08x] 0x%08x \n", i, (unsigned int)regMem[i]);
    }
  }
  
  SC_CTOR(memory) {
    FILE * fp;
    int i = 0;
    int aux;

    fp = fopen("memory.dat", "r");
    if(fp != NULL){
      while(!feof(fp) && i<MEM_SIZE){
        fscanf(fp, "%x", &aux);
        regMem[i] = aux;
        i++;
      }
      fclose(fp);
    }
    
    SC_METHOD(action);
    sensitive << clock.pos();
  }
  
};