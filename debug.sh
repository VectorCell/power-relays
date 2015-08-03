#!/bin/bash

# for testing on a machine without wiringPi.h

rm -f main-debug
g++ -pedantic -std=c++0x -Wall -MD -c main.c++ -DDEBUG
g++ -pedantic -std=c++0x -Wall main.o -o main-debug -DDEBUG
./main-debug $@
