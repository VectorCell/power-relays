#!/bin/bash

# for testing on a machine without wiringPi.h

outputfile="/run/shm/power-relays-debug-$RANDOM"

#CXXFLAGS="-pedantic -std=c++0x -Wall"
#CXXFLAGS="-pedantic -std=c++11 -Wall"

if make VARS=-DDEBUG EXECFILE=$outputfile ; then
	$outputfile
fi
