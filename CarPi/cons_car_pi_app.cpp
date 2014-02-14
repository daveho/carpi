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

#include "console.h"
#ifdef RASPBERRY_PI
#  include "button_input_reader_thread.h"
#endif
#include "cons_input_reader_thread.h"
#include "cons_menu_view.h"
#include "cons_music_player_view.h"
#include "cons_video_player_view.h"
#include "cons_car_pi_app.h"

ConsCarPiApp::ConsCarPiApp()
{
}

ConsCarPiApp::~ConsCarPiApp()
{
}

void ConsCarPiApp::initialize()
{
	setInstance(new ConsCarPiApp());
}

void ConsCarPiApp::onStartMainLoop()
{
	// Initialize console
	Console::initialize();
	Console::instance()->init();
	
	// Start the console input reader
	ConsInputReaderThread *inputReader = new ConsInputReaderThread();
	inputReader->start();

#ifdef RASPBERRY_PI
	// Start the button reader thread
	ButtonInputReaderThread *buttonInputReader = new ButtonInputReaderThread();
	if (buttonInputReader->initGpio()) {
		buttonInputReader->start();
	}
#endif
}

void ConsCarPiApp::onEndMainLoop()
{	
	// Clean up
	Console::instance()->cleanup();
}

EventHandler *ConsCarPiApp::createMenuView(Menu *menu)
{
	return new ConsMenuView(menu, 1, Console::instance()->getNumRows() - 2);
}

EventHandler *ConsCarPiApp::createMusicPlayerView(PlaySound *playSound)
{
	return new ConsMusicPlayerView(playSound);
}

EventHandler *ConsCarPiApp::createVideoPlayerView(PlayVideo *playVideo)
{
	return new ConsVideoPlayerView(playVideo);
}
