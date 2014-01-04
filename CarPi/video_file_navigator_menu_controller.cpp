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

#include "menu.h"
#include "string_util.h"
#include "video_file_navigator_menu_controller.h"

VideoFileNavigatorMenuController::VideoFileNavigatorMenuController(const std::string &baseDir)
	: FileNavigatorMenuController(baseDir)
{
}

VideoFileNavigatorMenuController::~VideoFileNavigatorMenuController()
{
}

bool VideoFileNavigatorMenuController::includeEntry(const std::string &entryName, int flags)
{
	// Include all directories
	if ((flags & MenuItem::FLAG_DIRECTORY) != 0) {
		return true;
	}
	
	// Include only video files (.m4v, .mpg, .avi)
	if (StringUtil::endsWith(entryName, ".m4v") || StringUtil::endsWith(entryName, ".mpg") || StringUtil::endsWith(entryName, ".avi")) {
		return true;
	}
	
	return false;
}
