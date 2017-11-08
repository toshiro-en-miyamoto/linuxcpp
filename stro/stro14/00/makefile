CC=g++
CCFLAG=-g -std=c++1z
LDFLAG=-g -lstdc++fs
DEP=
OBJ=hello.o
TARGET=a.out

%.o: %.cpp $(DEP)
	$(CC) $< -c $(CCFLAG)

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAG)

clean:
	rm $(TARGET) $(OBJ)