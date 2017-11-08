CC=g++
CCFLAG=-g -std=c++1z
LDFLAG=-g -lstdc++fs
DEP=b2tutil.h b2tutil_impl.h b2tedit.h
OBJ=b2t.o b2tutil.o b2tutil_impl.o b2tedit.o
TARGET=a.out

%.o: %.cpp $(DEP)
	$(CC) $< -c $(CCFLAG)

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAG)

clean:
	rm $(TARGET) $(OBJ)