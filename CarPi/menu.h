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

#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include <cstddef>

//
// Menu item class.
//
class MenuItem {
public:
	// Menu item flags.
	enum {
		FLAG_DIRECTORY = 1,
		FLAG_FILE = 2,
	};

private:
	std::string m_name;
	int m_value;
	int m_flags;
	
public:
	MenuItem(const std::string &name, int value, int flags = 0);
	~MenuItem();
	
	const std::string &getName() const { return m_name; }
	int getValue() const { return m_value; }
	int getFlags() const { return m_flags; }
	int hasFlag(int flag) const { return (m_flags & flag) != 0; }
};

//
// Abstract menu base class.
//
class Menu
{
public:
	Menu();
	virtual ~Menu();

	virtual size_t getNumItems() const = 0;
	virtual const MenuItem *getItem(size_t i) const = 0;
	virtual size_t getSelected() const = 0;
	virtual void setSelected(size_t i) = 0;
	virtual void clear() = 0;
	
	const MenuItem *getSelectedItem() const {
		return getItem(getSelected());
	}
};

#endif // MENU_H
