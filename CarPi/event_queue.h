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

#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <list>
#include <pthread.h>

class Event;

//
// Thread-safe event queue.
// Events can be enqueued from any thread.
//
class EventQueue
{
private:
	std::list<Event *> m_queue;
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;

	EventQueue();
	~EventQueue();
	
public:

	static void initialize();
	static EventQueue *instance();
	
	void enqueue(Event *evt);
	Event *dequeue();

	// Convenience method for scheduling a repaint.
	void repaint();
};

#endif // EVENTQUEUE_H
