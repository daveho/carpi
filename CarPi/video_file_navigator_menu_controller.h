#ifndef VIDEOFILENAVIGATORMENUCONTROLLER_H
#define VIDEOFILENAVIGATORMENUCONTROLLER_H

#include "file_navigator_menu_controller.h"

class VideoFileNavigatorMenuController : public FileNavigatorMenuController
{
public:
	VideoFileNavigatorMenuController(const std::string &baseDir);
	~VideoFileNavigatorMenuController();

	virtual bool includeEntry(const std::string &entryName, int flags);

};

#endif // VIDEOFILENAVIGATORMENUCONTROLLER_H
