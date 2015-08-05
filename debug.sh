#!/bin/bash

# for testing on a machine without wiringPi.h

outputfile="/run/shm/relays-debug-$RANDOM"

#cxxflags="-pedantic -std=c++0x -Wall"
cxxflags="-pedantic -std=c++11 -Wall"

g++ $cxxflags -MD -c main.c++ -DDEBUG
g++ $cxxflags main.o -o $outputfile -DDEBUG
rm -f *.o *.d
$outputfile $@
rm -f $outputfile
