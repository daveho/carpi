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

#ifndef NOTIFICATION_EVENT_FILTER_H
#define NOTIFICATION_EVENT_FILTER_H

#include "abstract_event_visitor.h"

//
// Delegating event handler that only sends notification
// events to the delegate.  We use this for cases such as
// editing playback settings, where the "parent" controller
// (e.g., music player controller) should receive notifications
// of event settings changes, even though a different
// controller/view are active.
//
class NotificationEventFilter : public AbstractEventVisitor {
private:
	EventHandler *m_delegate;

public:
	NotificationEventFilter(EventHandler *delegate);
	virtual ~NotificationEventFilter();

	virtual void visitNotificationEvent(NotificationEvent *evt);
};

#endif // NOTIFICATION_EVENT_FILTER_H
