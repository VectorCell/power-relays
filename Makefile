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
	LDFLAGS  := -pthread -lwiringPi
endif

all : $(EXECFILE) switch-monitor pwm-software

$(EXECFILE) : power-relays.cc power-relays.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) power-relays.cc -o $(EXECFILE) $(VARS)
	sudo chown root:root $(EXECFILE)
	sudo chmod +s $(EXECFILE)

switch-monitor : switch-monitor.c
	$(CC) $(CFLAGS) $(LDFLAGS) switch-monitor.c -o switch-monitor
	sudo chown root:root switch-monitor
	sudo chmod +s switch-monitor

pwm-software :
	$(CXX) $(CXXFLAGS) $(LDFLAGS) pwm-software.cc -o pwm-software $(VARS)
	sudo chown root:root pwm-software
	sudo chmod +s pwm-software

test : $(EXECFILE)
	./$(EXECFILE)
	sleep 1
	./$(EXECFILE) low
	sleep 1
	./$(EXECFILE) high
	sleep 1
	./$(EXECFILE) off
	sleep 1
	./$(EXECFILE) on

push-master :
	git push && git checkout master && git merge dev && git push && git checkout dev

clean :
	rm -f $(EXECFILE)
	rm -f switch-monitor
	rm -f power-relays-old
	rm -f pwm-software

-include *.d
