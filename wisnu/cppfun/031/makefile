CC=g++
CCFLAG=-std=c++14
DEP=vehicle.h
OBJ=vehicle.o lambda_tiny_func.o

%.o: %.cpp $(DEP)
	$(CC) -c $< $(CCFLAG)

a.out: $(OBJ)
	$(CC) -o $@ $^