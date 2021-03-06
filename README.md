# power-relays

A command-line tool for controlling relays attached to Rasbperry Pi gpio pins. Designed to be compiled to the C++11 standard, and dependent on [WiringPi](https://github.com/WiringPi/WiringPi).

Tested using the following compilers in the following operating systems:

| compiler | version | OS |
|----------|---------|----|
| g++-4.8 | 4.8.2 | Rasbpian 7.8 |
| g++ | 4.8.4 | elementary OS Freya, Ubuntu 14.04 |
| g++ | 4.6.3, using std=c++0x | elementary OS Luna, Ubuntu 12.04 |
| clang++ | 3.4 | Ubuntu 14.04 |

Compiled using the -DDEBUG flag in all cases, except on Rasbpian.

Travis CI builds:

|Branch      |Status   |
|------------|---------|
|master      | [![Build Status](https://travis-ci.org/VectorCell/power-relays.svg?branch=master)](https://travis-ci.org/VectorCell/power-relays?branch=master) |

Builds on Travis CI cannot test for full functionality with WiringPi.
