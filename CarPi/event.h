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

#ifndef EVENT_H
#define EVENT_H

class EventVisitor;

//
// Event base class.
//
class Event {
public:
	Event();
	virtual ~Event();
	
	virtual void accept(EventVisitor *visitor) = 0;
};

//
// Button event class.
// There are exactly 6 buttons.
//
class ButtonEvent : public Event {
public:
	enum Type {
		PRESS,
		RELEASE,
	};
	
	enum Code {
		UP,
		DOWN,
		LEFT,
		RIGHT,
		A,      // TODO: think of a better name
		B,      // TODO: think of a better name
	};
	
private:
	Type m_type;
	Code m_code;
	
public:
	ButtonEvent(Type type, Code code);
	virtual ~ButtonEvent();
	
	virtual void accept(EventVisitor *visitor);
	Type getType() const { return m_type; }
	Code getCode() const { return m_code; }
};

//
// Catch-all for various kinds of notifications.
//
class NotificationEvent : public Event {
public:
	enum Type {
		PAINT,             // render the view
		SELECTION_CHANGED, // menu selection changed
		MENU_CHANGED,      // menu changed (items added/removed)
	};
	
private:
	Type m_type;
	// add payload?
	
public:
	NotificationEvent(Type type);
	virtual ~NotificationEvent();
	
	Type getType() const { return m_type; }
	
	virtual void accept(EventVisitor *visitor);
};

class EventVisitor {
public:
	EventVisitor();
	virtual ~EventVisitor();
	virtual void visitButtonEvent(ButtonEvent *evt) = 0;
	virtual void visitNotificationEvent(NotificationEvent *evt) = 0;
};

#endif // EVENT_H