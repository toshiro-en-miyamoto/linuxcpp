CC=g++
CCFLAG=-std=c++14
DEP=vehicle.h
OBJ=vehicle.o find.o

%.o: %.cpp $(DEP)
	$(CC) -c $< $(CCFLAG)

a.out: $(OBJ)
	$(CC) -o $@ $^