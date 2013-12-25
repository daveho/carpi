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
#include "thread.h"
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

class InputReaderThread : public Thread {
public:
	InputReaderThread();
	virtual ~InputReaderThread();
	
	virtual void run();
};

InputReaderThread::InputReaderThread()
{
	// This thread should be detached: we can't easily stop it
	// because it will spend most of its time blocked waiting for
	// input from stdin.
	setDetached();
}

InputReaderThread::~InputReaderThread()
{
}

void InputReaderThread::run()
{
	// Note: it appears that it is not legal to call the ncurses getch() function
	// in this thread but execute the ncurses output functions in another thread.
	// So, we must read directly from stdin.  This actually isn't a great hardship:
	// the only issue will be decoding escape sequences (e.g., for the arrow keys.)
	for (;;) {
		int ch;
		ch = fgetc(stdin);
		if (ch < 0) {
			break;
		}
		switch (ch) {
			case 'w':
				EventQueue::instance()->enqueue(new ButtonEvent(ButtonEvent::RELEASE, ButtonEvent::UP));
				break;
			case 'a':
				EventQueue::instance()->enqueue(new ButtonEvent(ButtonEvent::RELEASE, ButtonEvent::LEFT));
				break;
			case 's':
				EventQueue::instance()->enqueue(new ButtonEvent(ButtonEvent::RELEASE, ButtonEvent::DOWN));
				break;
			case 'd':
				EventQueue::instance()->enqueue(new ButtonEvent(ButtonEvent::RELEASE, ButtonEvent::RIGHT));
				break;
			case 'q':
				EventQueue::instance()->enqueue(new ButtonEvent(ButtonEvent::RELEASE, ButtonEvent::A));
				break;
			case 'e':
				EventQueue::instance()->enqueue(new ButtonEvent(ButtonEvent::RELEASE, ButtonEvent::B));
				break;
		}
	}
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

void menuTest()
{
	EventQueue::initialize();

	Console::initialize();
	Console *cons = Console::instance();
	cons->init();
	
	InputReaderThread *inputReader = new InputReaderThread();
	inputReader->start();
	
	Menu *menu = new Menu();
	menu->addAndAdoptItem(new MenuItem("Apples", 0));
	menu->addAndAdoptItem(new MenuItem("Bananas", 1));
	menu->addAndAdoptItem(new MenuItem("Oranges", 2));
	
	MenuController *controller = new MenuController(menu);
	ConsMenuView *view = new ConsMenuView(menu);
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
