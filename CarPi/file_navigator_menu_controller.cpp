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

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "car_pi_app.h"
#include "event.h"
#include "menu.h"
#include "event_queue.h"
#include "file_navigator_menu_controller.h"

namespace {
	// If the given string starts with a non-negative integer prefix,
	// return it.  Otherwise, return -1.
	int numericPrefix(const std::string &s) {
		if (s.empty() || !isdigit(s[0])) {
			return -1;
		}
		int val = 0;
		size_t i = 0;
		while (i < s.size() && isdigit(s[i])) {
			val *= 10;
			val += (s[i] - '0');
			i++;
		}
		return val;
	}
	
	bool compareDirectoryEntries(MenuItem *left, MenuItem *right) {
		// Directories precede regular files
		bool leftIsDir = left->hasFlag(MenuItem::FLAG_DIRECTORY);
		bool rightIsDir = right->hasFlag(MenuItem::FLAG_DIRECTORY);
		if (leftIsDir != rightIsDir) {
			return leftIsDir;
		}
		
		// If both filenames start with an integer prefix,
		// and they are not the same number, use those to determine the
		// comparison order.  This ensures that, e.g.,
		// "10 - Mix Up the Satellites.mp3" comes *after*
		// "2 - My Son, My Secretary, My Country.mp3", rather
		// than before it (which is what would happen using
		// strict lexicographical order.
		int lnum = numericPrefix(left->getName());
		int rnum = numericPrefix(right->getName());
		if (lnum >= 0 && rnum >= 0 && lnum != rnum) {
			return lnum < rnum;
		}
		
		// Lexicographical comparison
		return left->getName() < right->getName();
	}
}

FileNavigatorMenuController::FileNavigatorMenuController(const std::string &baseDir)
	: MenuController(0) // populated during construction
{
	m_dirStack.push_back(baseDir);
	setMenu(new Menu());
	populateMenuItems();
}

FileNavigatorMenuController::~FileNavigatorMenuController()
{
}

void FileNavigatorMenuController::visitButtonEvent(ButtonEvent *evt)
{
	if (evt->getType() == ButtonEvent::RELEASE && evt->getCode() == ButtonEvent::LEFT) {
		setResult(EventHandler::HANDLED);
		if (m_dirStack.size() == 1) {
			// Go back to original menu
			CarPiApp::instance()->popEventHandler();
		} else {
			// Go back to parent directory
			m_dirStack.pop_back();
			size_t selectedItem = m_selectedItemStack.back();
			m_selectedItemStack.pop_back();
			onDirectoryChanged(selectedItem);
		}
	}
	
	if (!handled()) {
		Base::visitButtonEvent(evt);
	}
}

void FileNavigatorMenuController::visitNotificationEvent(NotificationEvent *evt)
{
	if (evt->getType() == NotificationEvent::MENU_ITEM_SELECTED) {
		const MenuItem *selectedItem = getMenu()->getSelectedItem();
		if (selectedItem->getValue() == PARENT_DIR_VALUE) {
			// Go back to parent directory
			setResult(EventHandler::HANDLED);
			m_dirStack.pop_back();
			
			// Restore selected item
			size_t selectedItem = m_selectedItemStack.back();
			m_selectedItemStack.pop_back();
			
			onDirectoryChanged(selectedItem);
		} else if (selectedItem->getValue() >= FIRST_FILE_VALUE) {
			if (selectedItem->hasFlag(MenuItem::FLAG_DIRECTORY)) {
				setResult(EventHandler::HANDLED);
				
				// Save current selected item
				m_selectedItemStack.push_back(getMenu()->getSelected());
				
				// Navigate into subdirectory
				m_dirStack.push_back(getFullPath(m_dirStack.back(), selectedItem->getName()));
				onDirectoryChanged(0);
			}
		}
	}
	
	if (!handled()) {
		Base::visitNotificationEvent(evt);
	}
}

bool FileNavigatorMenuController::includeEntry(const std::string &entryName, int flags)
{
	// Default implementation: include all files and directories
	return true;
}

void FileNavigatorMenuController::onMenuPopulated(Menu *menu)
{
	// Default implementation is a no-op
}

void FileNavigatorMenuController::populateMenuItems()
{
	assert(m_dirStack.size() > 0);
	std::string dirName = m_dirStack.back();

	Menu *menu = getMenu();
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
			// Skip dot files and directories.
			continue;
		}
		
		int flags = 0;
		
		std::string path(getFullPath(dirName, entry.d_name));

		// Check the entry type
		struct stat s;
		if (stat(path.c_str(), &s) != 0) {
			continue;
		}
		if (S_ISDIR(s.st_mode)) {
			flags |= MenuItem::FLAG_DIRECTORY;
		} else if (S_ISREG(s.st_mode)) {
			flags |= MenuItem::FLAG_FILE;
		} else {
			// Skip anything that's not a regular file or directory
			continue;
		}
		
		// Check whether this file or directory should be included
		if (!includeEntry(entry.d_name, flags)) {
			continue;
		}
		
		// Add the entry!
		menu->addAndAdoptItem(new MenuItem(entry.d_name, FIRST_FILE_VALUE + numEntries, flags));
		numEntries++;
	}
	
	closedir(dir);
	
	// Sort the menu items
	menu->sort(compareDirectoryEntries);
	
	// Allow the subclass to contribute additional menu items
	onMenuPopulated(menu);
}

void FileNavigatorMenuController::onDirectoryChanged(size_t selectedItem)
{
	populateMenuItems();
	if (getMenu()->getNumItems() > selectedItem) {
		getMenu()->setSelected(selectedItem);
	}
	EventQueue::instance()->enqueue(new NotificationEvent(NotificationEvent::MENU_CHANGED));
}

std::string FileNavigatorMenuController::getFullPath(const std::string &dirName, const std::string &entryName)
{
	std::string path(dirName);
	path += "/";
	path += entryName;
	return path;
}
