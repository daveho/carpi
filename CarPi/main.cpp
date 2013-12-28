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

#include <cstddef>
/*
#include <iostream>
#include <string>
#include "string_util.h"
*/
#include "car_pi_app.h"

/*
void st() {
	
	const std::string line("Ticket To Ride                Asylum Party                  The Grey Years Vol 2 (CD 1)   2006                              New Wave");

	std::string title, artist, album;
	
	size_t len = line.size();
	title = StringUtil::trimSpaces(line.substr(0, 30));
	artist = (len > 30) ? StringUtil::trimSpaces(line.substr(30, 30)) : "";
	album = (len > 60) ? StringUtil::trimSpaces(line.substr(60, 30)) : "";

	std::cout << "[" << title << "]" << std::endl;
	std::cout << "[" << artist << "]" << std::endl;
	std::cout << "[" << album << "]" << std::endl;
	
}
*/

int main(void)
{
//	st();
	CarPiApp::initialize();
	CarPiApp::instance()->mainLoop();
	return 0;
}
