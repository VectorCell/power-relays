ifeq ($(CXX), clang++)
	COVFLAGS := --coverage
	GCOV     := gcov-4.6
else
	CXX      := g++-4.8
	COVFLAGS := -fprofile-arcs -ftest-coverage
	GCOV     := gcov-4.8
endif

CXXFLAGS := -pedantic -std=c++11 -O3 -Wall
CFLAGS   := -std=c99 -O3 -Wall
LDFLAGS  := -lwiringPi
VALGRIND := valgrind

main : main.o Makefile main-old
	$(CXX) $(CXXFLAGS) $(LDFLAGS) main.o -o main
	rm -f *.d
	rm -f *.o

main-old : main.c
	gcc $(CFLAGS) $(LDFLAGS) main.c -o main-old

%.o : %.c++ Makefile
	$(CXX) $(CXXFLAGS) -MD -c $*.c++

test : Makefile
	g++ $(CXXFLAGS) -MD -c main.c++ -DDEBUG 2>&1
	g++ $(CXXFLAGS) -o main main.o 2>&1
	$(VALGRIND) ./main 2>&1

clean :
	rm -f *.d
	rm -f *.o
	rm -f main
	rm -f main-old

-include *.d
