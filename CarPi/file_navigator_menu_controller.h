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

#ifndef FILENAVIGATORMENUCONTROLLER_H
#define FILENAVIGATORMENUCONTROLLER_H

#include <string>
#include <vector>
#include "menu_controller.h"

class FileNavigatorMenuController : public MenuController
{
private:
	std::vector<std::string> m_dirStack;
	
public:
	enum {
		// menu item value for parent directory: "special" items should have lower values
		PARENT_DIR_VALUE = 98,
		
		// menu item value for special menu item created
		// when a directory cannot be read
		INVALID_DIR_VALUE = 99,
		
		// menu item value for first actual file or subdirectory
		FIRST_FILE_VALUE = 100,
	};

	FileNavigatorMenuController(const std::string &baseDir);
	~FileNavigatorMenuController();
	
private:
	void populateMenuItems();

};

#endif // FILENAVIGATORMENUCONTROLLER_H
