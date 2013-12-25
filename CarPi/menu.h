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

class MenuItem {
private:
	std::string m_name;
	int m_value;
	
public:
	MenuItem(const std::string &name, int value);
	~MenuItem();
	
	const std::string &getName() const { return m_name; }
	int getValue() const { return m_value; }
};

class Menu
{
private:
	std::vector<MenuItem *> m_itemList;
	size_t m_selected;
	
public:
	Menu();
	~Menu();

	void addAndAdoptItem(MenuItem *menuItem);
	size_t getNumItems() const { return size_t(m_itemList.size()); }
	MenuItem *getItem(size_t i) { return m_itemList.at(i); }
	size_t getSelected() const { return m_selected; }
	void setSelected(size_t i) { m_selected = i; }
};

#endif // MENU_H
