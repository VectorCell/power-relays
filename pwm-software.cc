/*
 * pwm-software.cc
 */

#include <iostream>
#include <thread>
#include <string>

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#include <wiringPi.h>


using namespace std;


#define  PERIOD  200

int pin = 0;
int duty_on = PERIOD;
int duty_off = 0;

bool running = true;


void sig_handler(int sig) {
	if (sig == SIGINT || sig == SIGTERM) {
		printf("Terminating due to signal ...\n");
		digitalWrite(pin, LOW);
		exit(0);
	}
}

void set_pwm_duty_percent(int percent) {
	assert(percent >= 0);
	assert(percent <= 100);
	duty_on = (percent * PERIOD) / 100;
	duty_off = PERIOD - duty_on;
	printf("duty_on:  %d\n", duty_on);
	printf("duty_off: %d\n", duty_off);
}

void pwm_software() {
	printf("beginning software pwm ...\n");
	while (running) {
		digitalWrite(pin, HIGH);
		delay(duty_on);
		digitalWrite(pin, LOW);
		delay(duty_off);
	}
}

int main (int argc, char *argv[]) {
	signal(SIGINT,  sig_handler);
	signal(SIGTERM, sig_handler);

	if (argc > 1) {
		pin = atoi(argv[1]);
	}
	if (argc > 2) {
		int percent = atoi(argv[2]);
		set_pwm_duty_percent(percent);
	} else {
		set_pwm_duty_percent(50);
	}

	if (wiringPiSetup() == -1) {
		printf("ERROR: unable to initialize GPIO.\n");
		return 1;
	}
	pinMode(pin, OUTPUT);

	thread worker_pwm(pwm_software);

	string line;
	while (getline(cin, line)) {
		int percent = atoi(line.c_str());
		set_pwm_duty_percent(percent);
	}
	printf("Terminating normally.\n");
	running = false;
	worker_pwm.join();

	return 0;
}
