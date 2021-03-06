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

#include "string_util.h"
#include "car_pi_app.h"
#include "menu.h"
#include "play_sound.h"
#include "menu.h"
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

void MusicFileNavigatorMenuController::onMenuPopulated(Menu *menu)
{
	// Add the special "<<Play all>>" item after the PARENT_DIR_VALUE item
	// or at the beginning of the menu (if the menu is the root of the
	// music directory).
	
	// First, check to see if there are any actual music files.
	// If not, then do nothing.
	bool hasFiles = false;
	for (size_t i = 0; i < menu->getNumItems(); i++) {
		if (menu->getItem(i)->hasFlag(MenuItem::FLAG_FILE)) {
			hasFiles = true;
			break;
		}
	}
	if (!hasFiles) {
		return;
	}
	
	// Find an appropriate insert position
	for (size_t i = 0; i < menu->getNumItems(); i++) {
		if (menu->getItem(i)->getValue() == FileNavigatorMenuController::PARENT_DIR_VALUE) {
			menu->insertAndAdoptItem(i+1, new MenuItem("<<Play all>>", PLAY_ALL_VALUE));
			return;
		}
	}
	
	// There was no parent dir item, so this must be the
	// root music directory.  In this case, insert the <<Play All>> item
	// at the beginning.
	menu->insertAndAdoptItem(0, new MenuItem("<<Play all>>", PLAY_ALL_VALUE));
}

void MusicFileNavigatorMenuController::visitButtonEvent(ButtonEvent *evt)
{
	if (evt->getType() == ButtonEvent::RELEASE && evt->getCode() == ButtonEvent::RIGHT) {
		const MenuItem *menuItem = getMenu()->getSelectedItem();
		if (menuItem->hasFlag(MenuItem::FLAG_FILE)) {
			setResult(EventHandler::HANDLED);
			
			// Play just the selected file
			PlaySound *playSound = new PlaySound();
			std::string path = getFullPath(getCurrentDir(), menuItem->getName());
			playSound->addFile(path);
			
			// Create player view/controller
			CarPiApp::instance()->startMusicPlayer(playSound);
			
			// Start player
			playSound->play(0);
		} else if (menuItem->getValue() == PLAY_ALL_VALUE) {
			setResult(EventHandler::HANDLED);
			
			// Play all files in the directory
			PlaySound *playSound = new PlaySound();
			for (size_t i = 0; i < getMenu()->getNumItems(); i++) {
				const MenuItem *menuItem = getMenu()->getItem(i);
				if (menuItem->hasFlag(MenuItem::FLAG_FILE)) {
					std::string path = getFullPath(getCurrentDir(), menuItem->getName());
					playSound->addFile(path);
				}
			}

			// Create player view/controller
			CarPiApp::instance()->startMusicPlayer(playSound);

			// Start playing the first file
			playSound->play(0);
		}
	}
	
	if (!handled()) {
		Base::visitButtonEvent(evt);
	}
}
