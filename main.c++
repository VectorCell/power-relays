/*
 * main.c++
 */

//#define DEBUG

#include <cstdio> // printf
#include <cstdint>
#include <cstring> // strcmp

#include <iostream> // cout
#include <utility> // make_pair
#include <vector>

#ifndef DEBUG
	#include <wiringPi.h>
#endif

using namespace std;

// states
#define LOW    0 // LOW and HIGH are actual pins states sent to the gpio pins
#define HIGH   1
#define OFF    2 // OFF and ON are logical states for practical purposes
#define ON     3
#define TOGGLE 4 // for switching from OFF and ON and vice-versa
typedef uint8_t state_t;

struct pin {
	uint8_t num;
	state_t on; // the state required for the pin to be "on", LOW or HIGH
	string name;
	pin (const uint8_t& n, const state_t& o, const string& na) : num(n), on(o), name(na) {}
};

struct change {
	pin p;
	state_t s; // the state that the pin will be changed to
	change (const pin& _p, const state_t& _s) : p(_p), s(_s) {}
};

bool init () {
	#ifndef DEBUG
		if (wiringPiSetup() == -1) {
			cerr << "ERROR: unable to access gpio pins" << endl;
			return false;
		}
	#endif
	return true;
}

void do_change (change& c) {
	if (c.s == OFF) {
		c.s = !c.p.on;
	} else if (c.s == ON) {
		c.s = c.p.on;
	}
	#ifndef DEBUG
		pinMode(c.p.num, OUTPUT);
		digitalWrite(c.p.num, (c.s != TOGGLE ? c.s : !digitalRead(c.p.num)));
	#else
		printf("setting pin %d to %d\n", c.p.num, c.s);
	#endif
}

int main (int argc, char *argv[]) {
	if (!init()) {
		return 1;
	}

	// here's where pin assigments are made
	// these are numbered according to the pin mapping used by wiringPi,
	// NOT the actual Broadcam numbers
	vector<pin> pins;
	pins.push_back(pin(12, HIGH, "top"));
	pins.push_back(pin(13, HIGH, "bottom"));
	pins.push_back(pin(15, HIGH, "leds"));

	if (argc <= 1) {
		cout << "usage:    relays STATE [PIN]" << endl;
		cout << "examples: relays on" << endl;
		cout << "          relays off" << endl;
		cout << "          relays [off|on|toggle] [top|bottom|leds]" << endl;
	} else {
		// expects state first
		state_t s;
		if (string(argv[1]) == "low") {
			s = LOW;
		} else if (string(argv[1]) == "high") {
			s = HIGH;
		} else if (string(argv[1]) == "off") {
			s = OFF;
		} else if (string(argv[1]) == "on") {
			s = ON;
		} else if (string(argv[1]) == "toggle") {
			s = TOGGLE;
		} else {
			cerr << "ERROR: unknown state: " << argv[1] << endl;
			return 1;
		}

		vector<change> changes;
		if (argc > 2 && (string(argv[2]) != "all")) {
			// pin specified
			bool found = false;
			for (pin& p : pins) {
				if (p.name == argv[2]) {
					found = true;
					changes.push_back(change(p, s));
				}
			}
			if (!found) {
				pin custom(atoi(argv[2]), HIGH, argv[2]);
				cerr << "WARNING: using user-specified pin " << static_cast<int>(custom.num);
				cerr << " (assuming that the pin's ON state is HIGH)" << endl;
				changes.push_back(change(custom, s));
			}
		} else {
			// pin not specified
			changes.push_back(change(pins[0], s));
			changes.push_back(change(pins[1], s));
		}
		for (change& c : changes) {
			do_change(c);
		}
	}

	return 0;
}
