#!/bin/bash

# for testing on a machine without wiringPi.h

outputfile="/run/shm/relays-debug-$RANDOM"

g++ -pedantic -std=c++0x -Wall -MD -c main.c++ -DDEBUG
g++ -pedantic -std=c++0x -Wall main.o -o $outputfile -DDEBUG
rm -f *.o *.d
$outputfile $@
rm -f $outputfile
