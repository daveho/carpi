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
