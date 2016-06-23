/*
* switch-monitor.c
*/

#include <stdlib.h> // exit
#include <signal.h>
#include <wiringPi.h>

#define P_SW        5
#define P_LED       4
#define P_PRECISIX 12
#define P_LIGHTS    7
#define P_AP        3

typedef struct pin_struct {
	int num;
	int onstate;
} pin_type;

pin_type output_pins[] = {
	{P_LED, 1},
	{P_PRECISIX, 1},
	{P_LIGHTS, 0},
	{P_AP, 0}
};

void sig_handler(int sig) {
	if (sig == SIGINT || sig == SIGTERM) {
		digitalWrite(P_LED, LOW);
		exit(0);
	}
}

void read_switch () {
	if (digitalRead(P_SW) == HIGH) {
		for (int k = 0; k < sizeof(output_pins) / sizeof(pin_type); ++k) {
			digitalWrite(output_pins[k].num, output_pins[k].onstate);
		}
	} else {
		digitalWrite(P_LED, LOW);
	}
}

int main (int argc, char *argv[]) {

	if (wiringPiSetup() == -1)
		return 1;
	pinMode(P_SW,     INPUT);
	for (int k = 0; k < sizeof(output_pins) / sizeof(pin_type); ++k) {
		pinMode(output_pins[k].num, OUTPUT);
	}
	wiringPiISR(P_SW, INT_EDGE_BOTH, read_switch);

	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);

	while (1) {
		delay(60000);
	}

	// unreachable
	return -1;
}
