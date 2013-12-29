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

#ifndef STATICMENU_H
#define STATICMENU_H

#include <vector>
#include "menu.h"

//
// Menu consisting of a static list of menu items.
//
class StaticMenu : public Menu
{
private:
	std::vector<MenuItem *> m_itemList;
	size_t m_selected;

public:
	StaticMenu();
	~StaticMenu();

	virtual void addAndAdoptItem(MenuItem *menuItem);
	virtual void insertAndAdoptItem(size_t pos, MenuItem *menuItem);

	virtual size_t getNumItems() const;
	virtual const MenuItem *getItem(size_t i) const;
	virtual size_t getSelected() const;
	virtual void setSelected(size_t i);
	virtual void clear();
	
	void sort(bool (*compareFunc)(MenuItem *left, MenuItem *right));
};

#endif // STATICMENU_H
