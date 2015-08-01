ifeq ($(CXX), clang++)
	COVFLAGS := --coverage
	GCOV     := gcov-4.6
else
	CXX      := g++-4.8
	COVFLAGS := -fprofile-arcs -ftest-coverage
	GCOV     := gcov-4.8
endif

CXXFLAGS := -pedantic -std=c++11 -Wall

main : main.o Makefile main-old
	g++-4.8 $(CXXFLAGS) -o main main.o -lwiringPi
	rm -f *.d
	rm -f *.o

main-old : main.c
	gcc -std=c99 -O3 -Wall main.c -o main-old -lwiringPi

%.o : %.c++ Makefile
	g++-4.8 $(CXXFLAGS) -MD -c $*.c++

test : Makefile main
	sudo ./main

clean :
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov
	rm -f *.d
	rm -f *.o
	rm -f main
	rm -f main-old

-include *.d
