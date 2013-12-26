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
#include <algorithm>
#include "static_menu.h"

StaticMenu::StaticMenu()
	: m_selected(0)
{
}

StaticMenu::~StaticMenu()
{
	for (std::vector<MenuItem *>::iterator i = m_itemList.begin(); i != m_itemList.end(); i++) {
		delete *i;
	}
}

void StaticMenu::addAndAdoptItem(MenuItem *menuItem)
{
	m_itemList.push_back(menuItem);
}

size_t StaticMenu::getNumItems() const
{
	return size_t(m_itemList.size());
}

const MenuItem *StaticMenu::getItem(size_t i) const
{
	return m_itemList.at(i);
}

size_t StaticMenu::getSelected() const
{
	return m_selected;
}

void StaticMenu::setSelected(size_t i)
{
	assert(i < m_itemList.size());
	m_selected = i;
}

void StaticMenu::clear()
{
	m_selected = 0;
	m_itemList.clear();
}

void StaticMenu::sort(bool (*compareFunc)(MenuItem *left, MenuItem *right))
{
	std::sort(m_itemList.begin(), m_itemList.end(), compareFunc);
}
