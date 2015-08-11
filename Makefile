ifeq ($(CXX), clang++)
	COVFLAGS := --coverage
	GCOV     := gcov-4.6
else
	COVFLAGS := -fprofile-arcs -ftest-coverage
	GCOV     := gcov-4.8
endif

ifeq ($(CXX), g++)
	CPP := g++-4.8
else
	CPP := $(CXX)
endif

ifndef EXECFILE
	EXECFILE := power-relays
endif

CXXFLAGS := -pedantic -std=c++11 -O3 -Wall
CFLAGS   := -std=c99 -O3 -Wall
ifndef VARS
	LDFLAGS  := -lwiringPi
endif
VALGRIND := valgrind

$(EXECFILE) : Makefile
	$(CPP) $(CXXFLAGS) $(LDFLAGS) power-relays.h power-relays.cc -o $(EXECFILE) $(VARS)

old :
	$(CC) $(CFLAGS) $(LDFLAGS) power-relays.c -o power-relays-old

test : Makefile $(EXECFILE)
	./$(EXECFILE)
	./$(EXECFILE) low
	./$(EXECFILE) high
	./$(EXECFILE) off
	./$(EXECFILE) on
	$(VALGRIND) --leak-check=full --show-reachable=yes ./main 2>&1

clean :
	rm -f $(EXECFILE)
	rm -f power-relays-old

-include *.d
