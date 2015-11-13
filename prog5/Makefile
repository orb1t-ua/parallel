CXX = mpic++
EXE = p5.out

P=4
M=10
N=10
G=3
 
INC_DIRS = 
CXXFLAGS = $(INC_DIRS) -std=c++11 -Wfatal-errors -Wall -O3
LDFLAGS = 
LDLIBS = 
 
LINK = $(CXX) -o
COMPILE = $(CXX) -c
 
CPP_FILES = $(wildcard *.cpp)
OBJ_FILES = $(CPP_FILES:.cpp=.o)
 
.PHONY: clean all run debug release
 
all:	$(EXE)

run:	$(EXE)
	mpiexec -n $(P) $(EXE) $(M) $(N) $(G)
	
clean:	
	rm *.o *.out

$(EXE):		$(OBJ_FILES)
	$(LINK) $(EXE) $(OBJ_FILES) $(LDFLAGS) $(LDLIBS) 
	
main.o: 	main.cpp
	$(COMPILE) $< $(CXXFLAGS)

%.o: 	%.cpp %.h
	$(COMPILE) $< $(CXXFLAGS)
