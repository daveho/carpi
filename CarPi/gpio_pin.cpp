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

#include <string>
#include <cassert>
#include <cstdio>
#include <ctime>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include "string_util.h"
#include "gpio_pin.h"

#define DEBUG(args...) printf(args)

namespace {
	void delay()
	{
		struct timespec ts;
		ts.tv_sec = 0;
		ts.tv_nsec = (1000000000L / 20);
		nanosleep(&ts, 0);
	}

	// Wait up to two seconds for a directory to exist
	DIR *waitForDirectory(const std::string &dirName)
	{
		DIR *dir;
		int count = 0;
		while (count < 20 && !(dir = opendir(dirName.c_str()))) {
			delay();
			count++;
		}
		return dir;
	}

	// Wait up to two seconds for a file to exist
	FILE *waitForFile(const std::string &fileName, const char *mode)
	{
		FILE *fh;
		int count = 0;
		while (count < 20 && !(fh = fopen(fileName.c_str(), mode))) {
			delay();
			count++;
		}
		return fh;
	}
}

// See:
// https://git.kernel.org/cgit/linux/kernel/git/torvalds/linux.git/tree/Documentation/gpio/gpio-legacy.txt

GpioPin::GpioPin()
	: m_pinNumber(-1)
	, m_valueFd(-1)
	, m_interruptMode(UNKNOWN)
{
}

GpioPin::~GpioPin()
{
	if (m_valueFd >= 0) {
		close(m_valueFd);
	}
}

bool GpioPin::initForReading(int pinNumber)
{
	assert(m_pinNumber < 0);
	m_pinNumber = pinNumber;

	// Each gpio pin (when exported) is represented by a directory
	std::string pinDir("/sys/class/gpio/gpio" + std::to_string(pinNumber));

	// Check to see if this gpio has been exported yet.
	// If not, request it to be exported.
	DIR *dir = opendir(pinDir.c_str());
	if (!dir) {
		// Try exporting the gpio
		FILE *exportFh = fopen("/sys/class/gpio/export", "w");
		if (!exportFh) {
			DEBUG("Export failed\n");
			return false;
		}
		fprintf(exportFh, "%d\n", pinNumber);
		fclose(exportFh);

		dir = waitForDirectory(pinDir);
		if (!dir) {
			DEBUG("Pin directory didn't appear\n");
			return false;
		}
	}
	closedir(dir);

	// Verify that the direction is set to "in"
	std::string directionPath(pinDir + "/direction");
	std::string direction;
	if (!StringUtil::readOneLine(directionPath, direction)) {
		DEBUG("Could not read from direction file\n");
		return false;
	}
	if (direction != "in") {
		// This is bad
		DEBUG("Direction is not set to in!\n");
		return false;
	}

	// Open the value file (for accessing the input pin's value)
	std::string valuePath(pinDir + "/value");
	FILE *valueFh = waitForFile(valuePath, "r");
	if (!valueFh) {
		DEBUG("Could not open value file\n");
		return false;
	}
	int valueFd = dup(fileno(valueFh)); // convert to file descriptor
	fclose(valueFh);

	// Success!
	m_valueFd = valueFd;
	return true;
}

bool GpioPin::initInterrupts(InterruptMode mode)
{
	assert(m_pinNumber >= 0);
	assert(m_valueFd >= 0);

	// Determine the mode string
	std::string modeStr;
	switch (mode) {
	case NONE:
		modeStr = "none"; break;
	case RISING:
		modeStr = "rising"; break;
	case FALLING:
		modeStr = "falling"; break;
	case BOTH:
		modeStr = "both"; break;
	default:
		// invalid mode
		DEBUG("Invalid mode\n");
		return false;
	}

	// Set the direction
	std::string edgePath("/sys/class/gpio/gpio" + std::to_string(m_pinNumber) + "/edge");
	FILE *edgeFh = waitForFile(edgePath, "w");
	if (!edgeFh) {
		DEBUG("Could not open edge file\n");
		return false;
	}
	fprintf(edgeFh, "%s\n", modeStr.c_str());
	fclose(edgeFh);

	// Verify that the direction was set correctly
	std::string edge;
	if (!StringUtil::readOneLine(edgePath, edge)) {
		DEBUG("Could not read from edge file\n");
		return false;
	}
	if (edge != modeStr) {
		DEBUG("Interrupt mode wasn't set correctly\n");
		return false;
	}

	// Success!
	return true;
}

int GpioPin::getValue()
{
	// Always seek to the beginning of the value file
	// to ensure that a fresh value is read.
	lseek(m_valueFd, off_t(0), SEEK_SET);

	// The value read will be either "0\n" or "1\n", so a fixed-sized
	// buffer should be fine.
	char buf[10];
	int rc = read(m_valueFd, buf, sizeof(buf));
	if (rc < 0) {
		return -1; // Couldn't read, hmm...
	}

	return buf[0] == '1'; // buf[0] should be either '0' or '1'
}

void GpioPin::makeFdSet(fd_set *fdSet, GpioPin *pinList, int numPins)
{
	FD_ZERO(fdSet);
	for (int i = 0; i < numPins; i++) {
		FD_SET(pinList[i].m_valueFd, fdSet);

	}
}
