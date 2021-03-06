/*
* power-relays.h
*/

#ifndef POWER_RELAYS_H
#define POWER_RELAYS_H

#include <cstdint>
#include <string>
#include <fstream>
#include <sstream>

#ifndef DEBUG
	#include <wiringPi.h>
#endif

// states
// LOW and HIGH are actual pins states sent to the gpio pins
// LOW and HIGH are already defined in wiringPi.h
// OFF and ON are logical states for practical purposes
// TOGGLE is a special state for switching from OFF and ON and vice-versa
// If any of these defines fail, it may require substantial changes to power-relays
#ifdef LOW
#	if LOW != 0
#		error "LOW is not defined as 0."
#	endif
#else
#	define LOW 0
#endif
#ifdef HIGH
#	if HIGH != 1
#		error "HIGH is not defined as 1."
#	endif
#else
#	define HIGH 1
#endif
#ifdef OFF
#	error "OFF already defined"
#else
#	define OFF 2
#endif
#ifdef ON
#	error "ON already defined"
#else
#	define ON 3
#endif
#ifdef TOGGLE
#	error "TOGGLE already defined"
#else
#	define TOGGLE 4
#endif
typedef uint_fast8_t state_type;

typedef uint_fast8_t pin_num_type;
struct pin {
	pin_num_type num;
	state_type on; // the state required for the pin to be "on", LOW or HIGH
	std::string name;
	pin (const pin_num_type& n, const state_type& o, const std::string& na) : num(n), on(o), name(na) {}
};

typedef std::function<void(const pin&)> action;

// returns either LOW or HIGH
state_type get_state (const pin& p);

void set_state (const pin& p, state_type s);

void print_state (const pin& p);

// returns either OFF or ON
state_type get_logical_state (const pin& p);

void set_logical_state (const pin& p, state_type s);

void print_logical_state (const pin& p);

void cycle_pin (const pin& p);

void print_status (const pin& p);

void init_pin (const pin& p);

bool init ();

// C should be an stl container of pins
// must support the push_back method
template <class C>
bool read_pins_file(const std::string& filename, C& destination) {
	using namespace std;
	ifstream reader;
	reader.open(filename);
	string line;
	string comment_prefix = "#";
	if (reader.is_open()) {
		while (getline(reader, line)) {
			if ((line.size() != 0) && !equal(comment_prefix.begin(), comment_prefix.end(), line.begin())) {
				istringstream ss(line);
				int num;
				ss >> num;
				int on;
				ss >> on;
				string name;
				ss >> name;
				destination.push_back(pin(num, on, name));
			}
		}
		reader.close();
		return true;
	} else {
		return false;
	}
}

#endif // end ifndef POWER_RELAYS_H
