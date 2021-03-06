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

#include "notification_event_filter.h"

NotificationEventFilter::NotificationEventFilter(EventHandler *delegate)
	: m_delegate(delegate)
{
}

NotificationEventFilter::~NotificationEventFilter()
{
	// Note that the delegate is NOT deleted
}

void NotificationEventFilter::visitNotificationEvent(NotificationEvent *evt)
{
	if (m_delegate->handleEvent(evt) == EventHandler::HANDLED) {
		setResult(EventHandler::HANDLED);
	}
}
