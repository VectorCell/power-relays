/*
* power-relays.h
*/

#ifndef __POWER_RELAYS_H__
#define __POWER_RELAYS_H__

#include <cstdint>

using namespace std;

// states
#define LOW    0 // LOW and HIGH are actual pins states sent to the gpio pins
#define HIGH   1
#define OFF    2 // OFF and ON are logical states for practical purposes
#define ON     3
#define TOGGLE 4 // special state for switching from OFF and ON and vice-versa
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

#endif // end ifndef __POWER_RELAYS_H__
