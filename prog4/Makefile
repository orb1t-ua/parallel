CXX = g++
EXE = p4.out

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
 
all: $(EXE)
 
debug:	CXXFLAGS += -DDEBUG -g
debug:	$(EXE)

release: CXXFLAGS += -O3
release: $(EXE)

parallel: CXXFLAGS += -O3 -fopenmp -DOMP_PARALLEL
parallel: LDLIBS += -fopenmp
parallel: $(EXE)

clean: 
	rm *.o $(EXE)

run:	$(EXE)
	./$(EXE) $(P) $(M) $(N) $(G)

$(EXE):		$(OBJ_FILES)
	$(LINK) $(EXE) $(OBJ_FILES) $(LDFLAGS) $(LDLIBS) 
	
main.o: 	main.cpp
	$(COMPILE) $< $(CXXFLAGS)

%.o: 	%.cpp %.h
	$(COMPILE) $< $(CXXFLAGS)
