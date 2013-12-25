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

#include "event.h"
#include "event_queue.h"
#include "menu.h"

MenuItem::MenuItem(const std::string &name, int value)
	: m_name(name)
	, m_value(value)
{
}

MenuItem::~MenuItem()
{
}

Menu::Menu()
	: m_selected(0)
{
}

Menu::~Menu()
{
	for (std::vector<MenuItem *>::iterator i = m_itemList.begin(); i != m_itemList.end(); i++) {
		delete *i;
	}
}

void Menu::addAndAdoptItem(MenuItem *menuItem)
{
	m_itemList.push_back(menuItem);
	EventQueue::instance()->enqueue(new NotificationEvent(NotificationEvent::MENU_CHANGED));
}
