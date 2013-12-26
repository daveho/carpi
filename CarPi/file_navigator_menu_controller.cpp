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
#include "static_menu.h"
#include "file_navigator_menu_controller.h"

FileNavigatorMenuController::FileNavigatorMenuController(const std::string &baseDir)
	: MenuController(0) // populated during construction
{
	m_dirStack.push_back(baseDir);
	populateMenuItems();
}

FileNavigatorMenuController::~FileNavigatorMenuController()
{
}

void FileNavigatorMenuController::populateMenuItems()
{
	assert(m_dirStack.size() > 0);
	std::string dirName = m_dirStack.back();
	
	if (getMenu() != 0) {
		delete getMenu();
	}
	StaticMenu *menu = new StaticMenu();
	setMenu(menu);
	
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
		menu->addAndAdoptItem(new MenuItem(entry.d_name, FIRST_FILE_VALUE + numEntries));
		numEntries++;
	}
	
	closedir(dir);
}
