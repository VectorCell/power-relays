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

CXXFLAGS := -pedantic -std=c++11 -O3 -Wall
CFLAGS   := -std=c99 -O3 -Wall
ifndef VARS
	LDFLAGS  := -lwiringPi
endif
VALGRIND := valgrind

main : main.o Makefile
	$(CPP) $(CXXFLAGS) $(LDFLAGS) main.o -o main $(VARS)
	rm -f *.d
	rm -f *.o

main-old : main.c
	$(CC) $(CFLAGS) $(LDFLAGS) main.c -o main-old

%.o : %.c++ Makefile
	$(CPP) $(CXXFLAGS) -MD -c $*.c++ $(VARS)

test : Makefile main
	./main
	./main off
	./main on
	./main off top
	./main on top
	./main off bottom
	./main on bottom
	$(VALGRIND) --leak-check=full ./main 2>&1

clean :
	rm -f *.d
	rm -f *.o
	rm -f main
	rm -f main-old

-include *.d
