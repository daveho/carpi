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

#ifndef CARPIAPP_H
#define CARPIAPP_H

#include <vector>

class EventHandler;
class Menu;

//
// Main application base class.
// Subclasses will provide implementations for specific user
// interfaces (e.g., console).
//
class CarPiApp
{
private:
	typedef std::vector<EventHandler *> HandlerStack;
	HandlerStack m_handlerStack;
	bool m_quit;
	
public:
	CarPiApp();
	virtual ~CarPiApp();

	static CarPiApp *instance();
	
	virtual void mainLoop();
	
	void quit();
	
	void pushEventHandler(EventHandler *handler);
	void popEventHandler();
	virtual void startMusicNavigator();
	
protected:
	static void setInstance(CarPiApp *theInstance);
	
	virtual void onStartMainLoop() = 0;
	virtual void onEndMainLoop() = 0;
	virtual EventHandler *createMenuView(Menu *menu) = 0;
};

#endif // CARPIAPP_H
