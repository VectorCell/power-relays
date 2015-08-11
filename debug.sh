#!/bin/bash

# for testing on a machine without wiringPi.h

outputfile="/run/shm/power-relays-debug-$RANDOM"

if make VARS=-DDEBUG EXECFILE=$outputfile ; then
	$outputfile
fi
