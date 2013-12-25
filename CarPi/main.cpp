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

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include "console.h"
#include "event.h"
#include "event_queue.h"
#include "abstract_event_visitor.h"
#include "cons_input_reader_thread.h"
#include "menu.h"
#include "menu_controller.h"
#include "cons_menu_view.h"
#include "play_sound.h"

void playSoundTest(int argc, char **argv)
{
	if (argc != 2) {
		std::cerr << "Usage: CarPi <path to sound file>" << std::endl;
		exit(1);
	}
	
	PlaySound ps;
	ps.play(argv[1]);
	
	bool done = false;
	while (!done) {
		std::string cmd;
		std::getline(std::cin, cmd);
		if (cmd == "pause") {
			ps.pause();
		} else if (cmd == "resume") {
			ps.resume();
		} else if (cmd == "quit") {
			ps.stop();
			ps.waitForIdle();
			done = true;
		}
	}
	
	std::cout << "Done!" << std::endl;
}

class QuitHandler : public AbstractEventVisitor {
private:
	bool m_done;
	
public:
	QuitHandler();
	virtual ~QuitHandler();
	
	bool isDone() const { return m_done; }
	
	virtual void visitButtonEvent(ButtonEvent *evt);
};

QuitHandler::QuitHandler()
	: m_done(false)
{
}

QuitHandler::~QuitHandler()
{
}

void QuitHandler::visitButtonEvent(ButtonEvent *evt)
{
	if (evt->getType() == ButtonEvent::RELEASE && evt->getCode() == ButtonEvent::A) {
		m_done = true;
	}
}

namespace {
	const char *s_menuItems[] = {
		"Apples",
		"Bananas",
		"Oranges",
		"Kumquats",
		"Persimmons",
		"Quinces",
		"Pomegranates",
		"Grapefruits",
		"Blueberries",
		"Strawberries",
		"Raspberries",
		"Blackberries",
		"Kiwis",
		"Pineapples",
		"Papayas",
		"Mangoes",
		"Cloudberries",
		"Cherries",
		"Plums",
		"Nectarines",
		"Tangerines",
		"Lemons",
		"Limes",
		"Plaintains",
	};
}

void menuTest()
{
	EventQueue::initialize();

	Console::initialize();
	Console *cons = Console::instance();
	cons->init();
	
	ConsInputReaderThread *inputReader = new ConsInputReaderThread();
	inputReader->start();
	
	Menu *menu = new Menu();
	for (int i = 0; i < int(sizeof(s_menuItems) / sizeof(const char*)); i++) {
		menu->addAndAdoptItem(new MenuItem(s_menuItems[i], i));
	}
	
	MenuController *controller = new MenuController(menu);
	ConsMenuView *view = new ConsMenuView(menu, 1, size_t(cons->getNumRows() - 2));
	QuitHandler *quitHandler = new QuitHandler();
	
	HandlerChain handlerChain;
	handlerChain.push_back(controller);
	handlerChain.push_back(view);
	handlerChain.push_back(quitHandler);
	
	EventQueue::instance()->enqueue(new NotificationEvent(NotificationEvent::PAINT));
	
	// Event loop!
	while (!quitHandler->isDone()) {
		Event *evt = EventQueue::instance()->dequeue();
		
		for (HandlerChain::iterator i = handlerChain.begin(); i != handlerChain.end(); i++) {
			EventHandler *handler = *i;
			if (handler->handleEvent(evt) == EventHandler::HANDLED) {
				break;
			}
		}
		
		delete evt;
	}
	
	cons->cleanup();
}

int main(int argc, char **argv)
{
	menuTest();
	return 0;
}
