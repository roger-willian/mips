#OBJ=main.o operational_unit.o functions.o
SOURCES= main.cpp functions.cpp
HEADERS= functions.h mips.h
EXE=sim
CXXLFLAGS= -L. -L$(SYSTEMC_HOME)/lib-linux64 -lsystemc -lm
CXXFLAGS= -I. -I$(SYSTEMC_HOME)/include
ifdef DEBUG
  DFLAG=-DDEBUG
endif

all: $(EXE)

$(EXE): $(SOURCES) $(HEADERS)
	g++ $(CXXFLAGS) $(CXXLFLAGS) $(SOURCES) $(DFLAG) -o $(EXE)

clean:
	rm -f *.o $(EXE)

