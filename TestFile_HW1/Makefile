SOURCE=bt.cpp md5.c
OPTFILE=btOpt.cpp md5.c
TARGET=bt
CC=g++
CFLAG=-O3
DFLAG=-O2 -g

all:
	$(CC) $(CFLAG) -o $(TARGET) $(SOURCE)

debug:
	$(CC) $(DFLAG) -o $(TARGET) $(SOURCE)

opt:
	$(CC) $(CFLAG) -o $(TARGET) $(OPTFILE)

clean:
	-rm $(TARGET)
