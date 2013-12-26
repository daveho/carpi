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

#include "composite_event_handler.h"

CompositeEventHandler::CompositeEventHandler()
{
}

CompositeEventHandler::CompositeEventHandler(EventHandler *controllerToAdopt, EventHandler *viewToAdopt)
{
	addAndAdoptEventHandler(controllerToAdopt);
	addAndAdoptEventHandler(viewToAdopt);
}

CompositeEventHandler::~CompositeEventHandler()
{
	for (HandlerList::iterator i = m_handlerList.begin(); i != m_handlerList.end(); i++) {
		delete *i;
	}
}

void CompositeEventHandler::addAndAdoptEventHandler(EventHandler *handler)
{
	m_handlerList.push_back(handler);
}

EventHandler::Result CompositeEventHandler::handleEvent(Event *evt)
{
	for (HandlerList::iterator i = m_handlerList.begin(); i != m_handlerList.end(); i++) {
		EventHandler::Result result = (*i)->handleEvent(evt);
		if (result == EventHandler::HANDLED) {
			return result;
		}
	}
	return EventHandler::NOT_HANDLED;
}
