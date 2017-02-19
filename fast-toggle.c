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

#define  DELAY 100
#define  PIN   4

void sig_handler(int sig) {
	if (sig == SIGINT || sig == SIGTERM) {
		digitalWrite(PIN, LOW);
		exit(0);
	}
}

int main (int argc, char *argv[]) {
	signal(SIGINT,  sig_handler);
	signal(SIGTERM, sig_handler);

	if (wiringPiSetup() == -1)
		return 1;

	pinMode(PIN, OUTPUT);
	uint64_t val = 0;
	while (1) {
		digitalWrite(PIN, val % 2 == 0);
		//printf("%d\n", (int)(val % 100));
		//delay(val % 100);
		++val;
	}

	return 0;
}
