/*
 * main.c++
 */

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
typedef uint_fast8_t state_t;

typedef uint_fast8_t pin_num_t;
struct pin {
	pin_num_t num;
	state_t on; // the state required for the pin to be "on", LOW or HIGH
	string name;
	pin (const pin_num_t& n, const state_t& o, const string& na) : num(n), on(o), name(na) {}
};

struct change {
	pin p;
	state_t s; // the state that the pin will be changed to
	change (const pin& _p, const state_t& _s) : p(_p), s(_s) {}
};

state_t get_state(const pin& p) {
	#ifndef DEBUG
		return digitalRead(p.num);
	#else
		return p.on;
	#endif
}

state_t get_logical_state(const pin& p) {
	state_t s = get_state(p);
	if (s == p.on) {
		return ON;
	} else {
		return OFF;
	}
}

void set_state(const pin& p, const state_t& s) {
	#ifndef DEBUG
		pinMode(p.num, OUTPUT);
		digitalWrite(p.num, (s != TOGGLE ? s : !get_state(p)));
	#else
		cout << "setting pin " << p.name << " (" << static_cast<int>(p.num) << ") to " << static_cast<int>(s) << endl;
	#endif
}

void do_change (change& c) {
	if (c.s == OFF) {
		c.s = !c.p.on;
	} else if (c.s == ON) {
		c.s = c.p.on;
	}
	set_state(c.p, c.s);
}

bool init () {
	#ifndef DEBUG
		if (wiringPiSetup() == -1) {
			cerr << "ERROR: unable to access gpio pins" << endl;
			return false;
		}
	#endif
	return true;
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
	pins.push_back(pin(1,  HIGH, "leds"));

	vector<pair<string, state_t>> state_names;
	state_names.push_back(make_pair("low",    LOW));
	state_names.push_back(make_pair("high",   HIGH));
	state_names.push_back(make_pair("off",    OFF));
	state_names.push_back(make_pair("on",     ON));
	state_names.push_back(make_pair("toggle", TOGGLE));

	if (argc <= 1) {
		for (pin& p : pins) {
			cout << (get_logical_state(p) == ON);
		}
		cout << endl;
		cout << "usage:    relays STATE [PIN]" << endl;
		cout << "examples: relays on" << endl;
		cout << "          relays off" << endl;
		cout << "          relays [off|on|toggle] [top|bottom|leds]" << endl;
	} else {
		// expects state first
		state_t s;
		bool valid_state = false;
		for (pair<string, state_t>& val : state_names) {
			if (val.first == argv[1]) {
				s = val.second;
				valid_state = true;
				break;
			}
		}
		if (!valid_state) {
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
