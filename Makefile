CXX=g++
CXXFLAGS=-std=c++14 -c -O3 -Wall -Werror -std=c++11 -Iinclude/ $(shell PKG_CONFIG_PATH=/usr/local/lib/pkgconfig/ pkg-config --static --cflags glfw3)
LDFLAGS=-std=c++14 -Llibs
LIBS=$(shell PKG_CONFIG_PATH=/usr/local/lib/pkgconfig/ pkg-config --static --libs glfw3) -lGL -lm -lfreeimage -lpthread -lglfw3
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXE=TheEye

default: all

all: $(OBJECTS) $(EXE)

debug: CXXFLAGS += -g
debug: CXXFLAGS := $(filter-out -O3,$(CXXFLAGS))
debug: EXE = $(EXE)-dbg
debug: all

$(EXE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ $(LIBS) -o $@ 

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@ $(LIBS)

clean:
	rm $(OBJECTS)
