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
#include <sys/ioctl.h>
#include <linux/tiocl.h>
#include <unistd.h>
#include "gpio_pin.h"
#include "event.h"
#include "event_queue.h"
#include "button_input_reader_thread.h"

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

	// Assignment of pins to button event codes
	const ButtonEvent::Code s_buttonCodes[] = {
		ButtonEvent::A,      // button 1
		ButtonEvent::UP,     // button 2
		ButtonEvent::B,      // button 3
		ButtonEvent::LEFT,   // button 4
		ButtonEvent::DOWN,   // button 5
		ButtonEvent::RIGHT,  // button 6
	};
}

ButtonInputReaderThread::ButtonInputReaderThread()
	: m_pinList(0)
	, m_last(~0) // assume that no buttons are pressed initially
{
	// Make this a detached thread: it will read the button input
	// until the process exits
	setDetached();
}

ButtonInputReaderThread::~ButtonInputReaderThread()
{
	delete[] m_pinList;
}

bool ButtonInputReaderThread::initGpio()
{
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
			ButtonEvent::Type type = ((cur & mask) != 0)
				? ButtonEvent::RELEASE
				: ButtonEvent::PRESS;
			ButtonEvent::Code code = s_buttonCodes[i];
			EventQueue::instance()->enqueue(new ButtonEvent(type, code));
			count++;
		}
	}
	m_last = cur;

	// If any events were generated, unblank the console
	// (in case screen blanking occurred.)
	if (count > 0) {
		char arg = TIOCL_UNBLANKSCREEN;
		ioctl(0, TIOCLINUX, &arg);
	}
}
