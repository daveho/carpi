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
	GpioPin *pinList = new GpioPin[NUM_BUTTONS];
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
	m_pinList = pinList;
	return true;
}

void ButtonInputReaderThread::run()
{
	if (m_pinList == 0) {
		return;
	}

	// Last input (assume all buttons are not pressed).
	// Note that 0=pressed.
	int last = ~(~0 << NUM_BUTTONS);

	fd_set pinSet;
	int highestFd = GpioPin::makeFdSet(&pinSet, m_pinList, NUM_BUTTONS);
	for (;;) {
		fd_set waitSet(pinSet);

		// Wait for a button to be pressed or released
		int rc = select(highestFd+1, 0, 0, &waitSet, 0);

		if (rc < 0) {
			// FIXME: should log
			sleep(1); // avoid burning CPU cycles
		} else {
			// Read updated state of buttons, generate
			// press/release events
			int cur = 0;
			for (int i = 0; i < NUM_BUTTONS; i++) {
				int value = m_pinList[i].getValue();
				if (value > 0) {
					cur |= (1 << i);
				}
				if ((cur & (1 << i)) != (last & (1 << i))) {
					// Button press/release
					if (cur & (1 << i)) {
						// release
						EventQueue::instance()->enqueue(new ButtonEvent(ButtonEvent::RELEASE, s_buttonCodes[i]));
					} else {
						// press
						EventQueue::instance()->enqueue(new ButtonEvent(ButtonEvent::PRESS, s_buttonCodes[i]));
					}
				}
			}
			last = cur;
		}
	}
}
