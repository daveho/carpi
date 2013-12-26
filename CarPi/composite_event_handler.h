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

#ifndef COMPOSITEEVENTHANDLER_H
#define COMPOSITEEVENTHANDLER_H

#include <vector>
#include "event_handler.h"

//
// Event handler that delegates to a series of
// other event handlers.  Useful for pairing a
// controller and a view.
//
class CompositeEventHandler : public EventHandler
{
private:
	typedef std::vector<EventHandler *> HandlerList;
	HandlerList m_handlerList;
	
public:
	CompositeEventHandler();
	CompositeEventHandler(EventHandler *controllerToAdopt, EventHandler *viewToAdopt);
	~CompositeEventHandler();

	void addAndAdoptEventHandler(EventHandler *handler);

	virtual Result handleEvent(Event *evt);
};

#endif // COMPOSITEEVENTHANDLER_H
