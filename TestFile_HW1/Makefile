SOURCE=bt.cpp md5.c
TARGET=bt
CC=g++
CFLAG=-O2
DFLAG=-O2 -g

all:
	$(CC) $(CFLAG) -o $(TARGET) $(SOURCE)

debug:
	$(CC) $(DFLAG) -o $(TARGET) $(SOURCE)

clean:
	-rm $(TARGET)
