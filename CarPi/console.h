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

#ifndef CONSOLE_H
#define CONSOLE_H

#include <cstddef>
#include <string>

class Console
{
private:
	int m_numRows;
	int m_numCols;
	int m_attr;

	Console();
	~Console();
	
public:
	enum Color {
		BLACK,      // 0000
		RED,        // 0001
		GREEN,      // 0010
		YELLOW,     // 0011
		BLUE,       // 0100
		MAGENTA,    // 0101
		CYAN,       // 0110
		GRAY,       // 0111
		INTENSE,    // 1000
	};
	
	static void initialize();
	static Console *instance();

	void init();
	void cleanup();
	int getNumRows();
	int getNumCols();
	void clear();
	void clearToEOL();
	void moveCursor(int row, int col);
	void attr(int bg, int fg);
	void print(const std::string &s);
	void printf(const char *fmt, ...);
	void commit();
	
private:
	void setAttrs();
};

#endif // CONSOLE_H
