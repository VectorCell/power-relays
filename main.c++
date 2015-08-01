/*
 * main.c++
 */

#include <cstdlib> // exit
#include <cstdio> // printf
#include <cstdint>
#include <cstring> // strcmp

#include <iostream> // cout
#include <utility> // make_pair
#include <vector>
#include <map>
#include <functional> // function

#include <wiringPi.h>

// pin assignments
// according to the pin mapping used by wiringPi,
// NOT the actual Broadcam numbers
#define P_TOP    12 // red wire
#define P_BOTTOM 13 // white wire
#define P_LEDS   15
#define P_ALL    255 // all except for P_LEDS
typedef uint8_t pin_t;

// states
// gets LOW = 0 and HIGH = 1 from wiringPi.h
#define OFF    LOW
#define ON     HIGH
#define TOGGLE 2 // HIGH is 1, LOW is 0
typedef uint8_t state_t;

using namespace std;

int main (int argc, char *argv[]) {
	if (wiringPiSetup() == -1)
		return 1;

	vector<pair<string, pin_t>> pins(3);
	pins.push_back(make_pair("top",    P_TOP));
	pins.push_back(make_pair("bottom", P_BOTTOM));
	pins.push_back(make_pair("leds",   P_LEDS));

	vector<pair<string, state_t>> states(3);
	states.push_back(make_pair("off",    OFF));
	states.push_back(make_pair("on",     ON));
	states.push_back(make_pair("toggle", TOGGLE));

	if (argc <= 1) {
		cout << "must specify either on or off" << endl;
		cout << "examples: relays on" << endl;
		cout << "          relays off" << endl;
		cout << "          relays [top|bottom|leds] [off|on]" << endl;
	} else {
		pin_t pin = P_ALL;
		state_t state = LOW;
		for (int c = 1; c < argc; ++c) {
			for (auto i = pins.begin(); i != pins.end(); ++i) {
				if ((*i).first == argv[c]) {
					pin = (*i).second;
				}
			}
			for (auto i = states.begin(); i != states.end(); ++i) {
				if ((*i).first == argv[c]) {
					state = (*i).second;
				}
			}
		}
		vector<pair<pin_t, state_t>> changes;
		if (pin == P_ALL) {
			changes.push_back(make_pair(P_TOP, state));
			changes.push_back(make_pair(P_BOTTOM, state));
		} else {
			changes.push_back(make_pair(pin, state));
		}
		for (auto i = changes.begin(); i != changes.end(); ++i) {
			pin_t p = (*i).first;
			state_t s = (*i).second;
			printf("setting pin %d to %d\n", p, (s != TOGGLE ? s : !digitalRead(p)));
			pinMode(p, OUTPUT);
			digitalWrite(p, s != TOGGLE ? s : !digitalRead(p));
		}
	}

	return 0;
}
