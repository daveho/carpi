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

#ifndef VIDEOFILENAVIGATORMENUCONTROLLER_H
#define VIDEOFILENAVIGATORMENUCONTROLLER_H

#include "file_navigator_menu_controller.h"

class VideoFileNavigatorMenuController : public FileNavigatorMenuController
{
private:
	typedef FileNavigatorMenuController Base;

public:
	VideoFileNavigatorMenuController(const std::string &baseDir);
	~VideoFileNavigatorMenuController();

	virtual bool includeEntry(const std::string &entryName, int flags);

	virtual void visitButtonEvent(ButtonEvent *evt);
};

#endif // VIDEOFILENAVIGATORMENUCONTROLLER_H
