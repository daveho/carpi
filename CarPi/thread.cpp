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

#include <cassert>
#include "thread.h"

Thread::Thread()
	: m_started(false)
	, m_finished(false)
{
}

Thread::~Thread()
{
}

void Thread::start()
{
	assert(!m_started);
	pthread_create(&m_thread, 0, &doRun, this);
	m_started = true;
}

void Thread::join()
{
	assert(m_started);
	assert(!m_finished);
	pthread_join(m_thread, 0);
	m_finished = true;
}

void *Thread::doRun(void *arg)
{
	Thread *t = static_cast<Thread *>(arg);
	t->run();
	return 0;
}
