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

#include "string_util.h"
#include "car_pi_app.h"
#include "menu.h"
#include "play_sound.h"
#include "music_player_controller.h"
#include "cons_music_player_view.h"
#include "composite_event_handler.h"
#include "music_file_navigator_menu_controller.h"

MusicFileNavigatorMenuController::MusicFileNavigatorMenuController(const std::string &baseDir)
	: FileNavigatorMenuController(baseDir)
{
}

MusicFileNavigatorMenuController::~MusicFileNavigatorMenuController()
{
}

bool MusicFileNavigatorMenuController::includeEntry(const std::string &entryName, int flags)
{
	// Include all directories
	if ((flags & MenuItem::FLAG_DIRECTORY) != 0) {
		return true;
	}
	
	// Include only music files (.mp3 or .ogg)
	if (StringUtil::endsWith(entryName, ".mp3") || StringUtil::endsWith(entryName, ".ogg")) {
		return true;
	}
	
	return false;
}

void MusicFileNavigatorMenuController::visitButtonEvent(ButtonEvent *evt)
{
	if (evt->getType() == ButtonEvent::RELEASE && evt->getCode() == ButtonEvent::RIGHT) {
		const MenuItem *menuItem = getMenu()->getSelectedItem();
		if (menuItem->hasFlag(MenuItem::FLAG_FILE)) {
			// Play file!
			std::string path = getFullPath(getCurrentDir(), menuItem->getName());
			PlaySound *playSound = new PlaySound();
			playSound->addFile(path);
			MusicPlayerController *controller = new MusicPlayerController(playSound);
			ConsMusicPlayerView *view = new ConsMusicPlayerView(playSound);
			CompositeEventHandler *pair = new CompositeEventHandler(controller, view);
			CarPiApp::instance()->pushEventHandler(pair);
			
			// At this point it should be safe to start playing, since
			// the controller and view are ready to receive events.
			playSound->play();
		}
	}
	
	if (!handled()) {
		Base::visitButtonEvent(evt);
	}
}
