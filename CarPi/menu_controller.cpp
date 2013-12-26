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
#include "menu_controller.h"

MenuController::MenuController(Menu *menu)
	: m_menu(menu)
{
}

MenuController::~MenuController()
{
}

void MenuController::visitButtonEvent(ButtonEvent *evt)
{
	// There is nothing to do if the menu has no items
	if (getMenu()->getNumItems() == 0) {
		return;
	}
	
	if (evt->getType() == ButtonEvent::RELEASE) {
		size_t selected = getMenu()->getSelected();
		
		switch (evt->getCode()) {
			case ButtonEvent::UP:
				setResult(EventHandler::HANDLED);
				if (selected > 0) {
					selected--;
				}
				break;
			
			case ButtonEvent::DOWN:
				setResult(EventHandler::HANDLED);
				if (selected < getMenu()->getNumItems() - 1) {
					selected++;
				}
				break;
				
			case ButtonEvent::RIGHT:
				setResult(EventHandler::HANDLED);
				EventQueue::instance()->enqueue(
					new NotificationEvent(NotificationEvent::MENU_ITEM_SELECTED, getMenu()->getSelectedItem()));
				break;
			
			default:
				break;
		}
		
		if (selected != getMenu()->getSelected()) {
			// Selection changed
			getMenu()->setSelected(selected);
			EventQueue::instance()->enqueue(new NotificationEvent(NotificationEvent::SELECTION_CHANGED));
		}
	}
}

void MenuController::visitNotificationEvent(NotificationEvent *evt)
{
	if (evt->getType() == NotificationEvent::MENU_ITEM_SELECTED) {
		const MenuItem *selectedItem = getMenu()->getSelectedItem();
		if (evt->getObject() == selectedItem) {
			setResult(EventHandler::HANDLED);
			onItemSelected(selectedItem);
		}
	}
	
	if (!handled()) {
		Base::visitNotificationEvent(evt);
	}
}

void MenuController::onItemSelected(const MenuItem *item)
{
	// Default implementation is a no-op: subclasses may override
}
