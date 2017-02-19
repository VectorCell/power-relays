ifeq ($(CXX), clang++)
	COVFLAGS := --coverage
	GCOV     := gcov-4.6
else
	COVFLAGS := -fprofile-arcs -ftest-coverage
	GCOV     := gcov-4.8
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

all : Makefile $(EXECFILE) switch-monitor

$(EXECFILE) : power-relays.cc power-relays.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) power-relays.cc -o $(EXECFILE) $(VARS)
	sudo chown root:root $(EXECFILE)
	sudo chmod +s $(EXECFILE)

switch-monitor : switch-monitor.c
	$(CC) $(CFLAGS) $(LDFLAGS) switch-monitor.c -o switch-monitor
	sudo chown root:root switch-monitor
	sudo chmod +s switch-monitor

old :
	$(CC) $(CFLAGS) $(LDFLAGS) power-relays.c -o power-relays-old
	sudo chown root:root power-relays-old
	sudo chmod +s power-relays-old

test : Makefile $(EXECFILE)
	./$(EXECFILE)
	./$(EXECFILE) low
	./$(EXECFILE) high
	./$(EXECFILE) off
	./$(EXECFILE) on
#	$(VALGRIND) --leak-check=full --show-reachable=yes ./$(EXECFILE) 2>&1

clean :
	rm -f $(EXECFILE)
	rm -f switch-monitor
	rm -f power-relays-old

-include *.d
