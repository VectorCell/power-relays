/*
* power-relays.cc
*/

#include <iostream> // cout
#include <utility> // make_pair
#include <string>
#include <vector>
#include <functional>

// for sleep functionality in cycle_pin
#include <chrono>
#include <thread>

#ifndef DEBUG
#	include <wiringPi.h>
#endif

#include "power-relays.h"

#ifndef PINCONF
#	define PINCONF "pins.conf"
#endif

using namespace std;

// returns either LOW or HIGH
state_type get_state (const pin& p) {
	#ifndef DEBUG
		return digitalRead(p.num);
	#else
		return p.on;
	#endif
}

void set_state (const pin& p, state_type s) {
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
	print_status(p);
}

void print_state (const pin& p) {
	cout << static_cast<int>(get_state(p));
}

// returns either OFF or ON
state_type get_logical_state (const pin& p) {
	state_type s = get_state(p);
	if (s == p.on) {
		return ON;
	} else {
		return OFF;
	}
}

void set_logical_state (const pin& p, state_type s) {
	set_state(p, s);
}

void print_logical_state (const pin& p) {
	cout << (get_logical_state(p) == ON);
}

void cycle_pin(const pin& p) {
	int n = 5;
	cout << "cycling pin, waiting for " << n << " seconds ... " << flush;
	set_state(p, TOGGLE);
	this_thread::sleep_for(chrono::milliseconds(n * 1000));
	set_state(p, TOGGLE);
	cout << "reset to original value." << endl;
}

void strobe_pin(const pin& p) {
	int n = 60;
	cout << "strobing pin for " << n << " seconds ... " << flush;


	state_type orig = get_state(p);
	#ifndef DEBUG
		bool clk = false;
		for (time_t t = time(NULL) + n; time(NULL) < t; ) {
			digitalWrite(p.num, (clk = !clk) ? HIGH : LOW);
			this_thread::sleep_for(chrono::milliseconds(50));
		}
	#endif
	set_state(p, orig);
	cout << "reset to original value." << endl;
}

void print_status(const pin& p) {
	cout << p.name << " " << (get_logical_state(p) == ON ? "on" : "off");
	cout << " (" << (get_state(p) == LOW ? "low" : "high") << ")" << endl;
}

void init_pin(const pin& p) {
	#ifndef DEBUG
		state_type s = get_state(p);
		pinMode(s, OUTPUT);
		set_state(p, s);
	#else
		cout << "called init_pin on pin " << p.name << endl;
	#endif
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

	vector<pin> pins;
	if (!read_pins_file(PINCONF, pins)) {
		cerr << "unable to open file: " << PINCONF << endl;
		return 1;
	}

	// if we need a new line at the end of output
	bool need_nl = false;

	vector<pair<string, action>> actions;
	actions.push_back(make_pair("low",    [] (const pin& p) -> void {set_state(p, LOW);}));
	actions.push_back(make_pair("high",   [] (const pin& p) -> void {set_state(p, HIGH);}));
	actions.push_back(make_pair("off",    [] (const pin& p) -> void {set_state(p, OFF);}));
	actions.push_back(make_pair("on",     [] (const pin& p) -> void {set_state(p, ON);}));
	actions.push_back(make_pair("toggle", [] (const pin& p) -> void {set_state(p, TOGGLE);}));
	actions.push_back(make_pair("cycle",  cycle_pin));
	actions.push_back(make_pair("strobe", strobe_pin));
	actions.push_back(make_pair("init",   init_pin));
	actions.push_back(make_pair("state",  [&need_nl] (const pin& p) -> void {print_logical_state(p); need_nl = true;}));
	actions.push_back(make_pair("status", print_status));


	if (argc <= 1) {
		cout << "usage:    relays ACTION [PIN]" << endl << endl;
		cout << "valid actions:" << endl;
		for (auto& p : actions) {
			cout << "\t" << p.first << endl;
		}
		cout << endl << "valid pins:" << endl;
		for (auto& p : pins) {
			cout << "\t" << p.name << endl;
		}
		cout << endl;
	} else {
		// expects state first
		action modify_state;
		bool valid_state = false;
		for (auto& val : actions) {
			if (val.first == argv[1]) {
				modify_state = val.second;
				valid_state = true;
				break;
			}
		}
		if (!valid_state) {
			cerr << "ERROR: unknown action: " << argv[1] << endl << endl;
			cout << "valid actions:" << endl;
			for (auto& p : actions) {
				cout << "\t" << p.first << endl;
			}
			cout << endl;
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
					cout << "ERROR: unknown pin: " << argv[k] << endl;
					cout << endl << "valid pins:" << endl;
					for (pin& p : pins) {
						cout << "\t" << p.name << endl;
					}
					cout << endl;
					return 1;
				}
			}
		} else {
			// pin not specified, apply action to them all
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
