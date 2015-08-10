/*
* main.c++
*/

#include <cstdint>

#include <iostream> // cout
#include <utility> // make_pair
#include <string>
#include <vector>
#include <functional>

#include <chrono>
#include <thread>

#ifndef DEBUG
	#include <wiringPi.h>
#endif

using namespace std;

// states
#define LOW    0 // LOW and HIGH are actual pins states sent to the gpio pins
#define HIGH   1
#define OFF    2 // OFF and ON are logical states for practical purposes
#define ON     3
#define TOGGLE 4 // special state for switching from OFF and ON and vice-versa
#define STATE  5 // special state signaling to print the state of the pin(s)
typedef uint_fast8_t state_t;

typedef uint_fast8_t pin_num_t;
struct pin {
	pin_num_t num;
	state_t on; // the state required for the pin to be "on", LOW or HIGH
	string name;
	pin (const pin_num_t& n, const state_t& o, const string& na) : num(n), on(o), name(na) {}
};

typedef function<void(const pin&)> action;

// returns either LOW or HIGH
state_t get_state (const pin& p) {
	#ifndef DEBUG
		return digitalRead(p.num);
	#else
		return p.on;
	#endif
}

void set_state (const pin& p, state_t s) {
	if (s == OFF) {
		s = !p.on;
	} else if (s == ON) {
		s = p.on;
	}
	#ifndef DEBUG
		pinMode(p.num, OUTPUT);
		digitalWrite(p.num, (s != TOGGLE ? s : !get_state(p)));
	#else
		cout << "setting pin " << p.name << " (" << static_cast<int>(p.num) << ") to " << static_cast<int>(s) << endl;
	#endif
}

void print_state (const pin& p) {
	cout << static_cast<int>(get_state(p));
}

// returns either OFF or ON
state_t get_logical_state (const pin& p) {
	state_t s = get_state(p);
	if (s == p.on) {
		return ON;
	} else {
		return OFF;
	}
}

void print_logical_state (const pin& p) {
	cout << (get_logical_state(p) == ON);
}

void strobe(const pin& p) {
	int n = 100;
	for (int k = 0; k < n; ++k) {
		cout << "party time with pin " << p.name << endl;
		set_state(p, TOGGLE);
		this_thread::sleep_for(chrono::milliseconds(100));
	}
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

	// if we need a new line at the end of output
	bool need_nl = false;

	vector<pair<string, action>> state_actions;
	state_actions.push_back(make_pair("low",    [] (const pin& p) -> void {set_state(p, LOW);}));
	state_actions.push_back(make_pair("high",   [] (const pin& p) -> void {set_state(p, HIGH);}));
	state_actions.push_back(make_pair("off",    [] (const pin& p) -> void {set_state(p, OFF);}));
	state_actions.push_back(make_pair("on",     [] (const pin& p) -> void {set_state(p, ON);}));
	state_actions.push_back(make_pair("toggle", [] (const pin& p) -> void {set_state(p, TOGGLE);}));
	state_actions.push_back(make_pair("strobe", strobe));
	state_actions.push_back(make_pair("state",  [&need_nl] (const pin& p) -> void {print_logical_state(p); need_nl = true;}));

	// here's where pin assigments are made
	// these are numbered according to the pin mapping used by wiringPi,
	// NOT the actual Broadcam numbers
	vector<pin> pins;
	pins.push_back(pin(12, HIGH, "precisix")); // 2-gang, top
	pins.push_back(pin(13, HIGH, "fan")); // 2-gang, bottom
	pins.push_back(pin(2,  LOW,  "tl")); // 4-gang, top left
	pins.push_back(pin(3,  LOW,  "tr")); // 4-gang, top right
	pins.push_back(pin(7,  LOW,  "lights-white")); // 4-gang, bottom left
	pins.push_back(pin(0,  LOW,  "lights-color")); // 4-gang, bottom right

	if (argc <= 1) {
		cout << "usage:    relays STATE [PIN]" << endl << endl;
		cout << "valid states:" << endl;
		for (auto& p : state_actions) {
			cout << "\t" << p.first << endl;
		}
		cout << endl << "valid pins:" << endl;
		for (auto& p : pins) {
			cout << "\t" << p.name << endl;
		}
		cout << endl;
//		cout << "examples: relays on" << endl;
//		cout << "          relays off" << endl;
//		cout << "          relays [off|on|toggle|state] [top|bottom|leds]" << endl;
	} else {
		// expects state first
		action modify_state;
		bool valid_state = false;
		for (pair<string, action>& val : state_actions) {
			if (val.first == argv[1]) {
				modify_state = val.second;
				valid_state = true;
				break;
			}
		}
		if (!valid_state) {
			cerr << "ERROR: unknown state: " << argv[1] << endl;
			return 1;
		}

		if (argc > 2) {
			for (int k = 2; k < argc; ++k) {
				bool found = false;
				for (pin& p : pins) {
					if (p.name == argv[k]) {
						found = true;
						modify_state(p);
					}
				}
				if (!found) {
					pin custom(atoi(argv[2]), HIGH, argv[2]);
					cerr << "WARNING: using user-specified pin " << static_cast<int>(custom.num);
					cerr << " (assuming that the pin's ON state is HIGH)" << endl;
					modify_state(custom);
				}
			}
		} else {
			// pin not specified, apply action to them all!
			for (pin& p : pins) {
				modify_state(p);
			}
		}
	}
	if (need_nl) {
		cout << endl;
	}
	return 0;
}
