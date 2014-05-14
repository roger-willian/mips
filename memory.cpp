#include "systemc.h"
#include <stdio.h>

#ifndef MEM_SIZE
#define MEM_SIZE 64
#endif

SC_MODULE(memory){
  sc_in_clk clock;
  sc_in< bool >         MemRead;
  sc_in< bool >         MemWrite;
  sc_in< sc_uint<32> >  address;
  sc_in< sc_uint<32> >  writeData;
  sc_out< sc_uint<32> > memData;
  
  sc_uint<8>           regMem[MEM_SIZE];
  
  void read(){
    unsigned int value;
    if (MemRead == 1){
      value = (regMem[address.read()(5,0)] << 24) |
              (regMem[address.read()(5,0) + 1] << 16) |
              (regMem[address.read()(5,0) + 2] << 8) |
              (regMem[address.read()(5,0) + 3]);
#ifdef DEBUG
      printf("MEM: reading 0x%08x from address 0x%08x\n", value, (unsigned int)address.read());
#endif
      memData.write(value);
    }    
  }
  
  void write(){
    if (MemWrite == 1){
#ifdef DEBUG
      printf("MEM: writing 0x%08x into address 0x%08x\n", (unsigned int)writeData.read(), (unsigned int)address.read());
#endif
      regMem[address.read()(5,0)] = writeData.read() >> 24;
      regMem[address.read()(5,0) + 1] = (writeData.read() & 0x00ff0000) >> 16;
      regMem[address.read()(5,0) + 2] = (writeData.read() & 0x0000ff00) >> 8;
      regMem[address.read()(5,0) + 3] = (writeData.read() & 0x000000ff);
    }
  }
  
  void dumpMem(){
    int i = 0;
    unsigned int value;
    puts("Memory contents:");
    for(i=0; i<MEM_SIZE; i+=4){
      value = (regMem[i] << 24) |
              (regMem[i + 1] << 16) |
              (regMem[i + 2] << 8) |
              (regMem[i + 3]);
      printf("[0x%08x] 0x%08x \n", i, value);
    }
  }
  
  SC_CTOR(memory) {
    FILE * fp;
    int i = 0;
    int aux;

    fp = fopen("memory.dat", "r");
    if(fp != NULL){
      while(!feof(fp) && i<MEM_SIZE){
        /*fscanf(fp, "%x", &aux);
        regMem[i] = aux >> 24;
        regMem[i + 1] = (aux & 0x00ff0000) >> 16;
        regMem[i + 2] = (aux & 0x0000ff00) >> 8;
        regMem[i + 3] = (aux & 0x000000ff);
        i+=4;*/
        regMem[i] = fgetc(fp);
        i++;
      }
      fclose(fp);
    }
    
    SC_METHOD(write);
    sensitive << clock.pos();
    
    SC_METHOD(read);
    sensitive << address;
    sensitive << MemRead;
  }
  
};