OBJ=main.o operational_unit.o functions.o
EXE=sim
CXXLFLAGS= -L. -L$(SYSTEMC_HOME)/lib-linux -lsystemc -lm
CXXFLAGS= -I. -I$(SYSTEMC_HOME)/include
ifdef DEBUG
  DFLAG=-DDEBUG
endif

all: $(EXE)

$(EXE): $(OBJ)
	g++ $(OBJ) $(CXXLFLAGS) -o $(EXE)

main.o: main.cpp functions.h operational_unit.cpp control_unit.o
	g++ $(CXXFLAGS) -c main.cpp $(DFLAG)

functions.o: functions.cpp functions.h
	g++ $(CXXFLAGS) -c functions.cpp $(DFLAG)

operational_unit.o: operational_unit.cpp functions.h instruction_register.cpp memory.cpp register_file.cpp register32.cpp ALU.cpp
	g++ $(CXXFLAGS) -c operational_unit.cpp $(DFLAG)

#control_unit.o: control_unit.cpp functions.h mips.h
#	g++ $(CXXFLAGS) -c control_unit.cpp $(DFLAG)

clean:
	rm -f *.o $(EXE)

