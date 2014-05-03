CC=g++
CFLAGS=-static -c -O3 -Wall -std=c++11 -I../include/ -L../libs/
DBGFLAGS=-static -c -Wall -Werror -std=c++11 -I../include/ -L../libs/
LDFLAGS=-static -I../include -L../libs
LIBS=-static-libgcc -lm -lfreeimage
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXE=TheEye

default: all

all: $(OBJECTS) $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -rf *.o
