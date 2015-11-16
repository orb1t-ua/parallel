CXX = mpic++
EXE = p5.out

P=8
M=10000
N=10000
G=20
 
INC_DIRS = 
CXXFLAGS = $(INC_DIRS) -std=c++11 -Wfatal-errors -Wall
LDFLAGS = 
LDLIBS = 
 
LINK = $(CXX) -o
COMPILE = $(CXX) -c
 
CPP_FILES = $(wildcard *.cpp)
OBJ_FILES = $(CPP_FILES:.cpp=.o)
 
.PHONY: clean all run debug release
 
all:	$(EXE)

debug:	CXXFLAGS += -DDEBUG -g
debug:	$(EXE)

release:	CXXFLAGS += -O3
release:	$(EXE)

run:	$(EXE)
	mpirun -n $(P) -H node0,node1 $(EXE) $(M) $(N) $(G)
	
clean:	
	rm *.o $(EXE)

$(EXE):		$(OBJ_FILES)
	$(LINK) $(EXE) $(OBJ_FILES) $(LDFLAGS) $(LDLIBS) 
	
main.o: 	main.cpp
	$(COMPILE) $< $(CXXFLAGS)

%.o: 	%.cpp %.h
	$(COMPILE) $< $(CXXFLAGS)
