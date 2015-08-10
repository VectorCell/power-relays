#!/bin/bash

# for testing on a machine without wiringPi.h

outputfile="/run/shm/relays-debug-$RANDOM"

CXXFLAGS="-pedantic -std=c++0x -Wall"
#CXXFLAGS="-pedantic -std=c++11 -Wall"

g++ $CXXFLAGS -MD -c main.c++ -DDEBUG
g++ $CXXFLAGS main.o -o $outputfile -DDEBUG
rm -f *.o *.d
$outputfile $@
rm -f $outputfile
