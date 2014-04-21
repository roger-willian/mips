OBJ=main.o mips.o functions.o
EXE=sim
CXXLFLAGS= -L. -L$(SYSTEMC_HOME)/lib-linux64 -lsystemc -lm
CXXFLAGS= -I. -I$(SYSTEMC_HOME)/include
ifdef DEBUG
  DFLAG=-DDEBUG
endif

all: $(EXE)

$(EXE): $(OBJ)
	g++ $(OBJ) $(CXXLFLAGS) -o $(EXE)

main.o: main.cpp functions.o mips.o
	g++ $(CXXFLAGS) -c main.cpp $(DFLAG)

functions.o: functions.cpp functions.h
	g++ $(CXXFLAGS) -c functions.cpp $(DFLAG)

mips.o: mips.cpp memory.cpp register_file.cpp instruction_register.cpp register32.cpp ALU.cpp
	g++ $(CXXFLAGS) -c mips.cpp $(DFLAG)




clean:
	rm -f *.o $(EXE)