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

#include <cassert>
#include "event.h"
#include "event_queue.h"

namespace {
	EventQueue *s_instance;
}

EventQueue::EventQueue()
{
	pthread_mutex_init(&m_mutex, 0);
	pthread_cond_init(&m_cond, 0);
}

EventQueue::~EventQueue()
{
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_cond);
}

void EventQueue::initialize()
{
	assert(s_instance == 0);
	s_instance = new EventQueue();
}

EventQueue *EventQueue::instance()
{
	assert(s_instance != 0);
	return s_instance;
}

void EventQueue::enqueue(Event *evt)
{
	pthread_mutex_lock(&m_mutex);
	m_queue.push_back(evt);
	pthread_cond_broadcast(&m_cond);
	pthread_mutex_unlock(&m_mutex);
}

Event *EventQueue::dequeue()
{
	Event *evt;
	
	pthread_mutex_lock(&m_mutex);
	while (m_queue.empty()) {
		pthread_cond_wait(&m_cond, &m_mutex);
	}
	evt = m_queue.front();
	m_queue.pop_front();
	pthread_mutex_unlock(&m_mutex);
	
	return evt;
}

void EventQueue::repaint()
{
	enqueue(new NotificationEvent(NotificationEvent::PAINT));
}
