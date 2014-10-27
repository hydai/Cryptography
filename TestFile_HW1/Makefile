SOURCE=bt.cpp md5.c
OPTFILE=btOpt.cpp md5.c
PRFILE=btPr.cpp md5.c
TARGET=bt
CC=g++
CFLAG=-O2
DFLAG=-O2 -g
PRFLAG=-O2 -lpthread

all:
	$(CC) $(CFLAG) -o $(TARGET) $(SOURCE)

debug:
	$(CC) $(DFLAG) -o $(TARGET) $(SOURCE)

opt:
	$(CC) $(CFLAG) -o $(TARGET) $(OPTFILE)

optdebug:
	$(CC) $(DFLAG) -o $(TARGET) $(OPTFILE)

pr:
	$(CC) $(PRFLAG) -o $(TARGET) $(PRFILE)

clean:
	-rm $(TARGET)
