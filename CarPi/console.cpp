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
#include <ncurses.h>
#include "console.h"

namespace {
	Console *s_instance;
}

Console::Console()
	: m_numRows(-1)
	, m_numCols(-1)
	, m_attr(0)
{
}

Console::~Console()
{
}

void Console::initialize()
{
	assert(s_instance == 0);
	s_instance = new Console();
}

Console *Console::instance()
{
	assert(s_instance != 0);
	return s_instance;
}

void Console::init()
{
	initscr();
	cbreak();
	noecho();
	nonl();
	
	getmaxyx(stdscr, m_numRows, m_numCols);

	start_color();
	
	// Initialize color pairs for the 64 "standard" color combinations.
	for (int i = 0; i < 64; i++) {
		init_pair(i+1, i&0x7, (i>>3)&0x7);
	}
}

void Console::cleanup()
{
	nl();
	echo();
	nocbreak();
	endwin();
}

int Console::getNumRows()
{
	return m_numRows;
}

int Console::getNumCols()
{
	return m_numCols;
}

void Console::clear()
{
	erase();
}

void Console::clearToEOL()
{
	clrtoeol();
}

void Console::moveCursor(int row, int col)
{
	move(row, col);
}

void Console::attr(int bg, int fg)
{
	m_attr = ((bg&0xF) << 4) | (fg&0xF);
}

void Console::print(const std::string &s)
{
	int bg = (m_attr>>4)&0xF;
	int fg = (m_attr&0xF);

	// Enable the color pair containing the "standard" color
	// (ignoring the INTENSE component).
	int pair = (((bg&7) << 3) | (fg&7)) + 1;
	attron(COLOR_PAIR(pair));

	// INTENSE foreground means A_BOLD.
	if ((fg & 8) != 0) {
		attron(A_BOLD);
	} else {
		attroff(A_BOLD);
	}
	
	// INTENSE background means A_BLINK.
	// Most terminal emulators interpret this as intense background
	// rather than actual blinking.
	if ((bg & 8) != 0) {
		attron(A_BLINK);
	} else {
		attroff(A_BLINK);
	}
	
	printw("%s", s.c_str());
}

void Console::commit()
{
	refresh();
}
