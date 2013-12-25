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

#include "menu.h"
#include "event_queue.h"
#include "string_util.h"
#include "console.h"
#include "cons_menu_view.h"

ConsMenuView::ConsMenuView(Menu *menu)
	: MenuView(menu)
{
}

ConsMenuView::~ConsMenuView()
{
}

EventHandler::Result ConsMenuView::handleEvent(Event *evt)
{
	evt->accept(this);
	return m_result;
}

void ConsMenuView::visitButtonEvent(ButtonEvent *evt)
{
	m_result = EventHandler::NOT_HANDLED;
	
	// There is nothing to do if the menu has no items
	if (getMenu()->getNumItems() == 0) {
		return;
	}
	
	if (evt->getType() == ButtonEvent::RELEASE) {
		size_t selected = getMenu()->getSelected();
		
		switch (evt->getCode()) {
			case ButtonEvent::UP:
				m_result = EventHandler::HANDLED;
				if (selected > 0) {
					selected--;
				}
				break;
			
			case ButtonEvent::DOWN:
				m_result = EventHandler::HANDLED;
				if (selected < getMenu()->getNumItems() - 1) {
					selected++;
				}
				break;
				
				// TODO: choosing an item
			
			default:
				break;
		}
		
		if (selected != getMenu()->getSelected()) {
			// Selection changed
			doPaint();
			EventQueue::instance()->enqueue(new NotificationEvent(NotificationEvent::SELECTION_CHANGED));
		}
	}
}

void ConsMenuView::visitNotificationEvent(NotificationEvent *evt)
{
	m_result = EventHandler::NOT_HANDLED;
	
	switch (evt->getType()) {
		case NotificationEvent::PAINT:
		case NotificationEvent::MENU_CHANGED:
		case NotificationEvent::SELECTION_CHANGED:
			m_result = EventHandler::HANDLED;
			doPaint();
			break;
		default:
			break;
	}
}

void ConsMenuView::doPaint()
{
	// TODO: handle scrolling
	Console *cons = Console::instance();
	Menu *menu = getMenu();
	
	size_t width = size_t(cons->getNumCols());
	
	cons->clear();
	for (size_t i = 0; i < menu->getNumItems(); i++) {
		cons->moveCursor(int(i), 0);
		if (i == menu->getSelected()) {
			cons->attr(Console::MAGENTA+Console::INTENSE, Console::GRAY+Console::INTENSE);
		} else {
			cons->attr(Console::BLACK, Console::GRAY);
		}
		cons->print(StringUtil::trimToSize(menu->getItem(i)->getName(), width-1));
	}
	
	cons->commit();
}
