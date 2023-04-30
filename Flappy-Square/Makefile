all: main

CXX = gcc
SRCS = main.cpp
override CXXFLAGS += -l SDL2 -lstdc++ -lm

main:
	$(CXX) $(CXXFLAGS) $(SRCS) -o "$@"

main-debug:
	$(CXX) $(CXXFLAGS) -O0 $(SRCS) -o "$@"

clean:
	rm -f main main-debug