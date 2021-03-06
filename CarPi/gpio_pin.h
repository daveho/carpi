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

#ifndef GPIO_PIN_H
#define GPIO_PIN_H

#include <sys/select.h>

class GpioPin {
public:
	enum InterruptMode {
		UNKNOWN,
		NONE,
		RISING,
		FALLING,
		BOTH,
	};

private:
	int m_pinNumber;
	int m_valueFd;
	InterruptMode m_interruptMode;

public:
	GpioPin();
	~GpioPin();

	bool initForReading(int pinNumber);
	bool isActive() const { return m_valueFd >= 0; }
	int getPinNumber() const { return m_pinNumber; }
	bool setInterruptMode(InterruptMode mode);
	InterruptMode getInterruptMode() const { return m_interruptMode; }
	int getValue() const;

	// Create an fd_set with the value file descriptors from
	// given GpioPin objects.  Returns the highest file descriptor.
	static int makeFdSet(fd_set *fdSet, GpioPin *pinList, int numPins);
};

#endif // GPIO_PIN_H
