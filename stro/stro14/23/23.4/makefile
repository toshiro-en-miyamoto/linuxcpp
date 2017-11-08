CC=g++
CCFLAG=-g -std=c++17
LDFLAG=-g
DEP=
OBJ=mailer.o mailfile.o message.o
TARGET=a.out

%.o: %.cpp $(DEP)
	$(CC) $< -c $(CCFLAG)

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAG)

clean:
	rm $(TARGET) $(OBJ)