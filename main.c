/*
 * main.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <wiringPi.h>

#define DELAY 2500

#define P_RED   12
#define P_WHITE 13
#define P_BOTH  255

#define TOGGLE 2 // HIGH is 1, LOW is 0

void sig_handler(int sig) {
	if (sig == SIGINT || sig == SIGTERM) {
		digitalWrite(P_RED, LOW);
		digitalWrite(P_WHITE, LOW);
		printf("exiting");
		exit(0);
	}
}

int main (int argc, char *argv[]) {
	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);

	if (wiringPiSetup() == -1)
		return 1;

	pinMode(P_RED, OUTPUT);
	pinMode(P_WHITE, OUTPUT);

	int val = -1;
	int pin = P_BOTH;
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "on") == 0) {
			val = HIGH;
		} else if (strcmp(argv[i], "off") == 0) {
			val = LOW;
		} else if (strcmp(argv[i], "toggle") == 0) {
			val = TOGGLE;
		} else if (strcmp(argv[i], "top") == 0) {
			pin = P_RED;
		} else if (strcmp(argv[i], "bottom") == 0) {
			pin = P_WHITE;
		} else {
			printf("unknown argument: %s\n", argv[i]);
		}
	}
	if (argc <= 1) {
		printf("must specify either on or off\n");
		printf("examples: relays on\n");
		printf("          relays off\n");
		printf("          relays top on\n");
		printf("          relays top off\n");
		printf("          relays bottom on\n");
		printf("          relays bottom off\n");
	}
	if (val != -1) {
		if (pin == P_BOTH) {
			printf("both relays");
			digitalWrite(P_RED, val == TOGGLE ? !digitalRead(P_RED) : val);
			digitalWrite(P_WHITE, val == TOGGLE ? !digitalRead(P_WHITE) : val);
		} else {
			printf("%s relay", pin == P_RED ? "top" : "bottom");
			digitalWrite(pin, val == TOGGLE ? !digitalRead(pin) : val);
		}
		printf(" set to ");
		switch (val) {
			case LOW:
				printf("off");
				break;
			case HIGH:
				printf("on");
				break;
			case TOGGLE:
				printf("toggle ");
				if (pin == P_BOTH) {
					printf("(now top is %s, bottom is %s)", digitalRead(P_RED) ? "on" : "off", digitalRead(P_WHITE) ? "on" : "off");
				} else {
					printf("(now %s is %s)", pin == P_RED ? "top" : "bottom", digitalRead(pin) ? "on" : "off");
				}
				break;
			default:
				printf("[ERROR: unknown pin state: %d", val);
		}
		printf("\n");
	}

	return 0 ;
}
