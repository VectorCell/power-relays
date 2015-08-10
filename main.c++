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
typedef uint_fast8_t state_type;

typedef uint_fast8_t pin_num_type;
struct pin {
	pin_num_type num;
	state_type on; // the state required for the pin to be "on", LOW or HIGH
	string name;
	pin (const pin_num_type& n, const state_type& o, const string& na) : num(n), on(o), name(na) {}
};

typedef function<void(const pin&)> action;

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

void print_status(const pin& p) {
	cout << p.name << " " << (get_logical_state(p) == ON ? "on" : "off");
	cout << " (" << (get_state(p) == LOW ? "low" : "high") << ")" << endl;
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

	vector<pair<string, action>> actions;
	actions.push_back(make_pair("low",    [] (const pin& p) -> void {set_state(p, LOW);}));
	actions.push_back(make_pair("high",   [] (const pin& p) -> void {set_state(p, HIGH);}));
	actions.push_back(make_pair("off",    [] (const pin& p) -> void {set_state(p, OFF);}));
	actions.push_back(make_pair("on",     [] (const pin& p) -> void {set_state(p, ON);}));
	actions.push_back(make_pair("toggle", [] (const pin& p) -> void {set_state(p, TOGGLE);}));
	actions.push_back(make_pair("cycle",  cycle_pin));
	actions.push_back(make_pair("state",  [&need_nl] (const pin& p) -> void {print_logical_state(p); need_nl = true;}));
	actions.push_back(make_pair("status", print_status));

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
					for (auto& p : pins) {
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
