// CarPi - Raspberry Pi car entertainment system
// Copyright (c) 2013, David H. Hovemeyer <david.hovemeyer@gmail.com>

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

#ifndef CONSINPUTREADERTHREAD_H
#define CONSINPUTREADERTHREAD_H

#include "thread.h"

//
// Thread to read console input events (from stdin) and
// post them to the event queue.  Runs as a detached
// thread, so it's not necessary (or possible) to join on it.
//
class ConsInputReaderThread : public Thread
{
public:
	ConsInputReaderThread();
	~ConsInputReaderThread();

	void run();
};

#endif // CONSINPUTREADERTHREAD_H
