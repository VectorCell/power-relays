/*
 * power-relays.c
 *
 * This is the old version. The new version is in power-relays.cc
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <wiringPi.h>

#define  DELAY  100
#define  PIN      0

void sig_handler(int sig) {
	if (sig == SIGINT || sig == SIGTERM) {
		digitalWrite(PIN, LOW);
		exit(0);
	}
}

int main (int argc, char *argv[]) {
	signal(SIGINT,  sig_handler);
	signal(SIGTERM, sig_handler);

	int percent = atoi(argv[1]);
	int duty_on = (percent * 200) / 100;
	int duty_off = 200 - duty_on;
	printf("duty_on:  %d\n", duty_on);
	printf("duty_off: %d\n", duty_off);

	if (wiringPiSetup() == -1)
		return 1;
	pinMode(PIN, OUTPUT);

	// digitalWrite(PIN, HIGH);
	// delay(2000);
	while (1) {
		digitalWrite(PIN, HIGH);
		delay(duty_on);
		digitalWrite(PIN, LOW);
		delay(duty_off);
	}

	return 0;
}
