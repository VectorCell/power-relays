/*
* switch-monitor.c
*/

#include <stdlib.h> // exit
#include <signal.h>
#include <wiringPi.h>

#define P_SW   5
#define P_LED  4
#define P_OUT 12

void sig_handler(int sig) {
	if (sig == SIGINT || sig == SIGTERM) {
		digitalWrite(P_LED, LOW);
		exit(0);
	}
}

void read_switch () {
	if (digitalRead(P_SW) == HIGH) {
		digitalWrite(P_LED, HIGH);
		digitalWrite(P_OUT, HIGH);
	} else {
		digitalWrite(P_LED, LOW);
	}
}

int main (int argc, char *argv[]) {
	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);

	if (wiringPiSetup() == -1)
		return 1;
	pinMode(P_SW,  INPUT);
	pinMode(P_LED, OUTPUT);
	pinMode(P_OUT, OUTPUT);
	wiringPiISR(P_SW, INT_EDGE_BOTH, read_switch);

	while (1) {
		delay(60000);
	}

	return 0;
}
