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

#ifndef BUTTON_INPUT_READER_THREAD_H
#define BUTTON_INPUT_READER_THREAD_H

#include <cstdint>
#include "thread.h"

class GpioPin;

//
// Thread to read button press/release events and generate
// key events.
//
class ButtonInputReaderThread : public Thread {
private:
	GpioPin *m_pinList;
	int m_last;
	int m_uinputFd;

public:
	ButtonInputReaderThread();
	virtual ~ButtonInputReaderThread();

	// Call this before starting the thread
	bool initGpio();

	virtual void run();

private:
	void sendEvent(uint16_t type, uint16_t code, int32_t value);
	void generateEvents();
};

#endif // BUTTON_INPUT_READER_THREAD_H
