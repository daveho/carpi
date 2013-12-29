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

ConsMenuView::ConsMenuView(Menu *menu, int topRow, int numRows)
	: MenuView(menu)
	, m_topRow(topRow)
	, m_numRows(numRows)
	, m_topItem(0)
{
}

ConsMenuView::~ConsMenuView()
{
}

void ConsMenuView::visitNotificationEvent(NotificationEvent *evt)
{
	switch (evt->getType()) {
		case NotificationEvent::PAINT:
			setResult(EventHandler::HANDLED);
			doPaint();
			break;
		case NotificationEvent::MENU_CHANGED:
		case NotificationEvent::SELECTION_CHANGED:
			setResult(EventHandler::HANDLED);
			onMenuChanged();
			doPaint();
			break;
		default:
			break;
	}
}

void ConsMenuView::onMenuChanged()
{
	int selected = int(getMenu()->getSelected());
	if (selected >= m_topItem && selected < (m_topItem + m_numRows)) {
		// selection is visible: no adjustments necessary
		return;
	} else if (selected < m_topItem) {
		// selection is above the top item: make it the top item
		m_topItem = selected;
	} else {
		// selection is below the last displayed item.
		// make the selection the last displayable item.
		m_topItem = (selected - m_numRows) + 1;
	}
}

void ConsMenuView::doPaint()
{
	Console *cons = Console::instance();
	const Menu *menu = getMenu();

	// Figure out where in the window the menu items will be painted
	int firstRow = m_topRow;
	int lastRow = m_topRow + m_numRows;
	if (lastRow > cons->getNumRows()) {
		lastRow = cons->getNumRows();
	}
	
	size_t item = m_topItem;
	for (int i = firstRow; i < lastRow; i++, item++) {
		cons->moveCursor(i, 0);
		if (item < menu->getNumItems()) {
			// Render the item
			if (item == menu->getSelected()) {
				// Highlight the selected item
				cons->attr(Console::MAGENTA, Console::GRAY+Console::INTENSE);
			} else {
				cons->attr(Console::BLACK, Console::GRAY);
			}
			const MenuItem *menuItem = menu->getItem(item);
			std::string itemName = menuItem->getName();
			if (menuItem->hasFlag(MenuItem::FLAG_DIRECTORY)) {
				itemName += "/";
			}
			std::string text = StringUtil::padWithSpaces(itemName, size_t(cons->getNumCols()));
			cons->print(text);
		} else {
			// Just clear the line
			cons->clearToEOL();
		}
	}

	cons->commit();
}
