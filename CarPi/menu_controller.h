#ifndef MENUCONTROLLER_H
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

#define MENUCONTROLLER_H

#include "abstract_event_visitor.h"

class Menu;
class MenuItem;

class MenuController : public AbstractEventVisitor
{
private:
	Menu *m_menu;
	
public:
	typedef AbstractEventVisitor Base;

	MenuController(Menu *menu);
	~MenuController();
	
	Menu *getMenu() { return m_menu; }

	virtual void visitButtonEvent(ButtonEvent *evt);
	virtual void visitNotificationEvent(NotificationEvent *evt);

	// No default implementation: subclasses can override
	virtual void onItemSelected(const MenuItem *item);
};

#endif // MENUCONTROLLER_H
