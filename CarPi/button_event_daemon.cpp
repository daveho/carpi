// CarPi - Raspberry Pi car entertainment system
// Copyright (c) 2013,2014 David H. Hovemeyer <david.hovemeyer@gmail.com>

// This file is part of CarPi.
// 
// CarPi is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
// 
// CarPi is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with CarPi.  If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>

#include "gpio_pin.h"

// Read button inputs on GPIO pins and use /dev/uinput to generate
// key events.

// Sources:
//   http://thiemonge.org/getting-started-with-uinput
//   http://www.geocities.ws/gnumohan/Linux/uinput.txt
//   http://lkcl.net/software/uinput/uinput.c

// Assignment of buttons to GPIO pins:
// Button 1: GPIO2 (P1-3)
// Button 2: GPIO3 (P1-5)
// Button 3: GPIO4 (P1-7)
// Button 4: GPIO17 (P1-11)
// Button 5: GPIO27 (P1-13)
// Button 6: GPIO22 (P1-15)
const int NUM_BUTTONS = 6;
const int gpioPins[NUM_BUTTONS] = { 2, 3, 4, 17, 27, 22 };

// Assignment of pins to button event codes
const int g_buttonCodes[] = {
	KEY_Q,
	KEY_W,
	KEY_E,
	KEY_A,
	KEY_S,
	KEY_D,
};

int g_uinputFd;
GpioPin g_pinList[NUM_BUTTONS];
int g_highestFd;
fd_set g_pinSet;
int g_last = ~(~0 << NUM_BUTTONS);

void setupUinput()
{
	g_uinputFd = open("/dev/uinput", O_WRONLY | O_NDELAY);
	if (g_uinputFd < 0) {
		fprintf(stderr, "Error opening /dev/uinput\n");
		exit(1);
	}

	// Register the key events we will send
	if (ioctl(g_uinputFd, UI_SET_EVBIT, EV_KEY) < 0
		|| ioctl(g_uinputFd, UI_SET_KEYBIT, KEY_Q) < 0
		|| ioctl(g_uinputFd, UI_SET_KEYBIT, KEY_W) < 0
		|| ioctl(g_uinputFd, UI_SET_KEYBIT, KEY_E) < 0
		|| ioctl(g_uinputFd, UI_SET_KEYBIT, KEY_A) < 0
		|| ioctl(g_uinputFd, UI_SET_KEYBIT, KEY_S) < 0
		|| ioctl(g_uinputFd, UI_SET_KEYBIT, KEY_D) < 0
		) {
		fprintf(stderr, "Error registering uinput events\n");
		exit(1);
	}

	// Register the device
	struct uinput_user_dev uinputDev;
	memset(&uinputDev, 0, sizeof(uinputDev));
	snprintf(uinputDev.name, UINPUT_MAX_NAME_SIZE, "carpi-buttons");
	uinputDev.id.bustype = BUS_USB;
	uinputDev.id.vendor  = 0x1234;
	uinputDev.id.product = 0xfedc;
	uinputDev.id.version = 1;
	if (write(g_uinputFd, &uinputDev, sizeof(uinputDev)) < 0) {
		fprintf(stderr, "Error sending uinput device data\n");
		exit(1);
	}
	if (ioctl(g_uinputFd, UI_DEV_CREATE) < 0) {
		fprintf(stderr, "Error registering uinput device\n");
		exit(1);
	}
}

void setupGpio()
{
	for (int i = 0; i < NUM_BUTTONS; i++) {
		if (!g_pinList[i].initForReading(gpioPins[i])) {
			fprintf(stderr, "Couldn't init pin %d for reading\n", gpioPins[i]);
			exit(1);
		}
		if (!g_pinList[i].setInterruptMode(GpioPin::BOTH)) {
			fprintf(stderr, "Couldn't set interrupts for pin %d\n", gpioPins[i]);
			exit(1);
		}
	}

	g_highestFd = GpioPin::makeFdSet(&g_pinSet, g_pinList, NUM_BUTTONS);
}

void generateEvents()
{
	// Read updated state of buttons, generate
	// press/release events
	int cur = 0;
	for (int i = 0, mask = 1; i < NUM_BUTTONS; i++, mask <<= 1) {
		int value = g_pinList[i].getValue();
		if (value > 0) {
			cur |= mask;
		}
		if ((cur & mask) != (g_last & mask)) {
			// Button press/release
			struct input_event evt;
			memset(&evt, 0, sizeof(evt));
			evt.type = EV_KEY;
			evt.code = g_buttonCodes[i];
			evt.value = ((cur & mask) != 0) ? 0 : 1; // 0=release, 1=press
			printf("Generating %s for button %d\n", evt.value?"press":"release", i);
			write(g_uinputFd, &evt, sizeof(evt));
		}
	}
	g_last = cur;
}

int main(void)
{
	setupUinput();
	setupGpio();

	// Input loop
	for (;;) {
		// Wait for a button to be pressed or released
		fd_set waitSet(g_pinSet);
		int rc = select(g_highestFd+1, 0, 0, &waitSet, 0);

		if (rc < 0) {
			// FIXME: should log
			sleep(1); // avoid burning CPU cycles
		} else {
			// Input pin or pins changed, so generate events
			generateEvents();
		}
	}

	// Doesn't exit, but can be terminated by a signal
}
