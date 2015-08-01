ifeq ($(CXX), clang++)
	COVFLAGS := --coverage
	GCOV     := gcov-4.6
else
	CXX      := g++-4.8
	COVFLAGS := -fprofile-arcs -ftest-coverage
	GCOV     := gcov-4.8
endif

CXXFLAGS := -pedantic -std=c++11 -Wall

main : main.o Makefile
	g++-4.8 $(CXXFLAGS) -o main main.o -lwiringPi
	rm -f *.d
	rm -f *.o

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

-include *.d
