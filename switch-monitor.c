/*
* switch-monitor.c
*/

#include <stdlib.h> // exit
#include <signal.h>
#include <wiringPi.h>

#define P_SW      5
#define P_LED     4
#define P_COMP   12
#define P_LIGHTS  7
#define P_FAN     3

void sig_handler(int sig) {
	if (sig == SIGINT || sig == SIGTERM) {
		digitalWrite(P_LED, LOW);
		exit(0);
	}
}

void read_switch () {
	if (digitalRead(P_SW) == HIGH) {
		digitalWrite(P_LED, HIGH);

		digitalWrite(P_COMP,   HIGH);
		digitalWrite(P_LIGHTS, LOW);
		//digitalWrite(P_FAN,    LOW);
	} else {
		digitalWrite(P_LED, LOW);
	}
}

int main (int argc, char *argv[]) {

	if (wiringPiSetup() == -1)
		return 1;
	pinMode(P_SW,     INPUT);
	pinMode(P_LED,    OUTPUT);
	pinMode(P_COMP,   OUTPUT);
	pinMode(P_LIGHTS, OUTPUT);
	pinMode(P_FAN,    OUTPUT);
	wiringPiISR(P_SW, INT_EDGE_BOTH, read_switch);

	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);

	while (1) {
		delay(60000);
	}

	// unreachable
	return -1;
}
