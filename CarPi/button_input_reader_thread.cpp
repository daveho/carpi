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

#include <memory>
#include <cstdio>
#include <cstring>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <fcntl.h>
#include <unistd.h>
#include "gpio_pin.h"
#include "button_input_reader_thread.h"

// Notes:
//
// This thread is a bit weird.  You might ask why it doesn't add
// ButtonEvents to the event queue directly, and that's a good
// question.  The reason is that the LCD backlight is powered off
// if no input events occur for a period of time, and there does
// not seem to be any way to explicitly power it back on!  So,
// this thread reads the button events and turns them into
// genuine key input events using /dev/uinput.  Those key events
// in turn are read by ConsInputReaderThread, and as a side effect
// they will restore power to the LCD backlight if it has been
// powered off.  That's the idea, anyway.

namespace {
	// Assignment of buttons to GPIO pins:
	// Button 1: GPIO2 (P1-3)
	// Button 2: GPIO3 (P1-5)
	// Button 3: GPIO4 (P1-7)
	// Button 4: GPIO17 (P1-11)
	// Button 5: GPIO27 (P1-13)
	// Button 6: GPIO22 (P1-15)
	const int NUM_BUTTONS = 6;
	const int s_gpioPins[NUM_BUTTONS] = { 2, 3, 4, 17, 27, 22 };

	// Assignment of pins to key event codes
	uint16_t s_buttonCodes[] = {
		KEY_Q,
		KEY_W,
		KEY_E,
		KEY_A,
		KEY_S,
		KEY_D,
	};
}

ButtonInputReaderThread::ButtonInputReaderThread()
	: m_pinList(0)
	, m_last(~0) // assume that no buttons are pressed initially
	, m_uinputFd(-1)
{
	// Make this a detached thread: it will read the button input
	// until the process exits
	setDetached();
}

ButtonInputReaderThread::~ButtonInputReaderThread()
{
	delete[] m_pinList;
	if (m_uinputFd >= 0) {
		ioctl(m_uinputFd, UI_DEV_DESTROY);
		close(m_uinputFd);
	}
}

bool ButtonInputReaderThread::initGpio()
{
	// Initialize gpio pins for input and interrupts on both
	// rising and falling edges
	std::unique_ptr<GpioPin[]> pinList(new GpioPin[NUM_BUTTONS]);
	for (int i = 0; i < NUM_BUTTONS; i++) {
		// Open the pin for reading
		if (!pinList[i].initForReading(s_gpioPins[i])) {
			return false;
		}
		// Set up pin for interrupts on rising and falling edges
		if (!pinList[i].setInterruptMode(GpioPin::BOTH)) {
			return false;
		}
	}

	// Register a uinput device to turn button events into key events
	m_uinputFd = open("/dev/uinput", O_WRONLY | O_NDELAY);
	if (m_uinputFd < 0) {
		return false;
	}
	if (ioctl(m_uinputFd, UI_SET_EVBIT, EV_KEY) < 0
		|| ioctl(m_uinputFd, UI_SET_KEYBIT, KEY_Q) < 0
		|| ioctl(m_uinputFd, UI_SET_KEYBIT, KEY_W) < 0
		|| ioctl(m_uinputFd, UI_SET_KEYBIT, KEY_E) < 0
		|| ioctl(m_uinputFd, UI_SET_KEYBIT, KEY_A) < 0
		|| ioctl(m_uinputFd, UI_SET_KEYBIT, KEY_S) < 0
		|| ioctl(m_uinputFd, UI_SET_KEYBIT, KEY_D) < 0) {
		return false;
	}
	struct uinput_user_dev uinputDev;
	memset(&uinputDev, 0, sizeof(uinputDev));
	snprintf(uinputDev.name, UINPUT_MAX_NAME_SIZE, "carpi-buttons");
	uinputDev.id.bustype = BUS_USB;
	uinputDev.id.vendor  = 0x1234;
	uinputDev.id.product = 0xfedc;
	uinputDev.id.version = 1;
	if (write(m_uinputFd, &uinputDev, sizeof(uinputDev)) < 0) {
		return false;
	}
	if (ioctl(m_uinputFd, UI_DEV_CREATE) < 0) {
		return false;
	}

	// Successful initialization!
	m_pinList = pinList.release();
	return true;
}

void ButtonInputReaderThread::run()
{
	if (m_pinList == 0) {
		return;
	}

	// Make a fd_set of the pin value file descriptors
	fd_set pinSet;
	int highestFd = GpioPin::makeFdSet(&pinSet, m_pinList, NUM_BUTTONS);

	// Input loop
	for (;;) {
		// Wait for a button to be pressed or released
		fd_set waitSet(pinSet);
		int rc = select(highestFd+1, 0, 0, &waitSet, 0);

		if (rc < 0) {
			// FIXME: should log
			sleep(1); // avoid burning CPU cycles
		} else {
			// Input pin or pins changed, so generate events
			generateEvents();
		}
	}
}

void ButtonInputReaderThread::sendEvent(uint16_t type, uint16_t code, int32_t value)
{
	struct input_event evt;
	memset(&evt, 0, sizeof(evt));
	evt.type = type;
	evt.code = code;
	evt.value = value;
	write(m_uinputFd, &evt, sizeof(evt));
}

void ButtonInputReaderThread::generateEvents()
{
	int count = 0;

	// Read updated state of buttons, generate
	// press/release events
	int cur = 0;
	for (int i = 0, mask = 1; i < NUM_BUTTONS; i++, mask <<= 1) {
		int value = m_pinList[i].getValue();
		if (value > 0) {
			cur |= mask;
		}
		if ((cur & mask) != (m_last & mask)) {
			// Button press/release
			int32_t value = ((cur & mask) != 0) ? 0 : 1; // 0=release,1=press
			sendEvent(EV_KEY, s_buttonCodes[i], value);
			count++;
		}
	}
	m_last = cur;

	// Were any events generated?
	if (count > 0) {
		// Send an EV_SYN event.
		// I think the idea is that this allows the program
		// that generates the input events to explicitly control when
		// the events are pushed to consumers.  In our case we
		// definitely want them to be pushed immediately.
		sendEvent(EV_SYN, SYN_REPORT, 0);
	}
}
