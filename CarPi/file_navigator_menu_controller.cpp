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

#include <cassert>
#include <sys/types.h>
#include <dirent.h>
#include "event.h"
#include "static_menu.h"
#include "event_queue.h"
#include "file_navigator_menu_controller.h"

FileNavigatorMenuController::FileNavigatorMenuController(const std::string &baseDir)
	: MenuController(0) // populated during construction
{
	m_dirStack.push_back(baseDir);
	setMenu(new StaticMenu());
	populateMenuItems();
}

FileNavigatorMenuController::~FileNavigatorMenuController()
{
}

void FileNavigatorMenuController::visitNotificationEvent(NotificationEvent *evt)
{
	if (evt->getType() == NotificationEvent::MENU_ITEM_SELECTED) {
		const MenuItem *selectedItem = getMenu()->getSelectedItem();
		if (selectedItem->getValue() == PARENT_DIR_VALUE) {
			// Go back to parent directory
			setResult(EventHandler::HANDLED);
			m_dirStack.pop_back();
			onDirectoryChanged();
		} else if (selectedItem->getValue() >= FIRST_FILE_VALUE) {
			setResult(EventHandler::HANDLED);
			std::string path(m_dirStack.back());
			path += "/";
			path += selectedItem->getName();
			m_dirStack.push_back(path);
			onDirectoryChanged();
		}
	}
	
	if (!handled()) {
		Base::visitNotificationEvent(evt);
	}
}

void FileNavigatorMenuController::populateMenuItems()
{
	assert(m_dirStack.size() > 0);
	std::string dirName = m_dirStack.back();

	StaticMenu *menu = static_cast<StaticMenu *>(getMenu());
	menu->clear();
	
	if (m_dirStack.size() > 1) {
		menu->addAndAdoptItem(new MenuItem("..", PARENT_DIR_VALUE));
	}
	
	DIR *dir = opendir(dirName.c_str());
	if (!dir) {
		menu->addAndAdoptItem(new MenuItem("Error reading directory", INVALID_DIR_VALUE));
		return;
	}
	
	int numEntries = 0;
	for (;;) {
		struct dirent entry, *p;
		int rc = readdir_r(dir, &entry, &p);
		if (rc != 0 || p == 0) {
			break;
		}
		if (entry.d_name[0] == '.') {
			continue;
		}
		menu->addAndAdoptItem(new MenuItem(entry.d_name, FIRST_FILE_VALUE + numEntries));
		numEntries++;
	}
	
	closedir(dir);
}

void FileNavigatorMenuController::onDirectoryChanged()
{
	populateMenuItems();
	EventQueue::instance()->enqueue(new NotificationEvent(NotificationEvent::MENU_CHANGED));
}
