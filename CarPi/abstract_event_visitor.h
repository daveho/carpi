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

#ifndef ABSTRACTEVENTVISITOR_H
#define ABSTRACTEVENTVISITOR_H

#include "event.h"
#include "event_handler.h"

//
// Default EventVisitor/EventHandler implementation that results in
// EventVisitor::NOT_HANDLED for every event.  Subclasses can
// then handle just the events that are interesting to them.
//
class AbstractEventVisitor : public EventVisitor, public EventHandler
{
private:
	Result m_result;
	
public:
	AbstractEventVisitor();
	~AbstractEventVisitor();
	
	void setResult(Result result) { m_result = result; }
	Result getResult() const { return m_result; }
	bool handled() const { return m_result == EventHandler::HANDLED; }

	virtual Result handleEvent(Event *evt);

	virtual void visitButtonEvent(ButtonEvent *evt);
	virtual void visitNotificationEvent(NotificationEvent *evt);
};

#endif // ABSTRACTEVENTVISITOR_H
