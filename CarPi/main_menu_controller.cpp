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

#include <cstdlib>
#include "car_pi_app.h"
#include "main_menu.h"
#include "main_menu_controller.h"

MainMenuController::MainMenuController()
	: MenuController(new MainMenu())
{
}

MainMenuController::~MainMenuController()
{
}

void MainMenuController::onItemSelected(const MenuItem *item)
{
	MainMenu::ItemValue value = static_cast<MainMenu::ItemValue>(item->getValue());
	switch (value) {
		case MainMenu::VIDEOS:
			CarPiApp::instance()->startVideoNavigator();
			break;
		case MainMenu::MUSIC:
			CarPiApp::instance()->startMusicNavigator();
			break;
		case MainMenu::SETTINGS:
			break;
		case MainMenu::SHELL:
			break;
		case MainMenu::SHUT_DOWN:
			system("/usr/bin/sudo /sbin/shutdown -h now");
			//CarPiApp::instance()->quit();
			break;
	}
}
