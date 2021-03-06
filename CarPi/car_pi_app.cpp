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
#include <cassert>
#include "music_file_navigator_menu_controller.h"
#include "video_file_navigator_menu_controller.h"
#include "event_queue.h"
#include "main_menu_controller.h"
#include "video_player_controller.h"
#include "music_player_controller.h"
#include "composite_event_handler.h"
#include "notification_event_filter.h"
#include "playback_settings.h"
#include "playback_settings_controller.h"
#include "car_pi_app.h"

namespace {
	CarPiApp *s_instance;
}

CarPiApp::CarPiApp()
	: m_quit(false)
{
}

CarPiApp::~CarPiApp()
{
}

CarPiApp *CarPiApp::instance()
{
	assert(s_instance != 0);
	return s_instance;
}

void CarPiApp::mainLoop()
{
	// Create event queue
	EventQueue::initialize();
	
	// Allow subclass to do any UI-specific initialization
	onStartMainLoop();
	
	// Create the main menu controller/view and push it
	MainMenuController *mainMenuController = new MainMenuController();
	EventHandler *mainMenuView = createMenuView(mainMenuController->getMenu());
	CompositeEventHandler *pair = CompositeEventHandler::makeAdoptedPair(mainMenuController, mainMenuView);
	pushEventHandler(pair);
	
	// Schedule the initial paint event
	EventQueue::instance()->enqueue(new NotificationEvent(NotificationEvent::PAINT));
	
	// Event loop!
	while (!m_quit) {
		Event *evt = EventQueue::instance()->dequeue();
		m_handlerStack.back()->handleEvent(evt);
		delete evt;
	}
	
	// Allow subclass to do UI-specific cleanup
	onEndMainLoop();
}

void CarPiApp::quit()
{
	m_quit = true;
}

void CarPiApp::setInstance(CarPiApp *theInstance)
{
	assert(s_instance == 0);
	s_instance = theInstance;
}

void CarPiApp::startMusicNavigator()
{
	std::string musicDir;
	
	musicDir += getenv("HOME");
	musicDir += "/Music";
	
	MusicFileNavigatorMenuController *controller = new MusicFileNavigatorMenuController(musicDir);
	EventHandler *view = createMenuView(controller->getMenu());
	
	CompositeEventHandler *pair = CompositeEventHandler::makeAdoptedPair(controller, view);
	
	pushEventHandler(pair);
}

void CarPiApp::startMusicPlayer(PlaySound *playSound)
{
	MusicPlayerController *controller = new MusicPlayerController(playSound);
	EventHandler *view = createMusicPlayerView(playSound);
	CompositeEventHandler *pair = CompositeEventHandler::makeAdoptedPair(controller, view);
	pushEventHandler(pair);
}

void CarPiApp::startVideoNavigator()
{
	std::string musicDir;
	
	musicDir += getenv("HOME");
	musicDir += "/Videos";
	
	VideoFileNavigatorMenuController *controller = new VideoFileNavigatorMenuController(musicDir);
	EventHandler *view = createMenuView(controller->getMenu());
	
	CompositeEventHandler *pair = CompositeEventHandler::makeAdoptedPair(controller, view);
	
	pushEventHandler(pair);
}

void CarPiApp::startVideoPlayer(PlayVideo *playVideo)
{
	VideoPlayerController *controller = new VideoPlayerController(playVideo);
	EventHandler *view = createVideoPlayerView(playVideo);
	CompositeEventHandler *pair = CompositeEventHandler::makeAdoptedPair(controller, view);
	pushEventHandler(pair);
}

void CarPiApp::startPlaybackSettingsEditor(EventHandler *currentController, PlaybackSettings *playbackSettings)
{
	// This works a little differently than a normal switch to
	// a child controller and view: we allow the "parent" controller
	// to continue handling notification events.
	CompositeEventHandler *triple = new CompositeEventHandler();
	triple->addHandler(new NotificationEventFilter(currentController), true);
	triple->addHandler(new PlaybackSettingsController(playbackSettings), true);
	triple->addHandler(createPlaybackSettingsView(playbackSettings), true);
	pushEventHandler(triple);
}

void CarPiApp::pushEventHandler(EventHandler *handler)
{
	m_handlerStack.push_back(handler);
	EventQueue::instance()->enqueue(new NotificationEvent(NotificationEvent::PAINT));
}

void CarPiApp::popEventHandler()
{
	EventHandler *handler = m_handlerStack.back();
	m_handlerStack.pop_back();
	delete handler;
	EventQueue::instance()->enqueue(new NotificationEvent(NotificationEvent::PAINT));
}
