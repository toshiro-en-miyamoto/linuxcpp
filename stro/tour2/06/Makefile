CC=c++-10
CCFLAG=-g -std=c++2a
# LDFLAG=-g -stdlib=libc++
OBJ=*.o
TARGET=a.out

%.o: %.cpp
	$(CC) $< -c $(CCFLAG)

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAG)
	dsymutil $(TARGET)

clean:
	rm $(TARGET) $(OBJ)
	rm -rf $(TARGET).dSYM