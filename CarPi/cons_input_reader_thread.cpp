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
#include <map>
#include "event.h"
#include "event_queue.h"
#include "cons_input_reader_thread.h"

ConsInputReaderThread::ConsInputReaderThread()
{
	// This thread should be detached: we can't easily stop it
	// because it will spend most of its time blocked waiting for
	// input from stdin.
	setDetached();
}

ConsInputReaderThread::~ConsInputReaderThread()
{
}

void ConsInputReaderThread::run()
{
	// Map of key values to ButtonEvent code values.
	typedef std::map<int, ButtonEvent::Code> ButtonCodeMap;
	ButtonCodeMap buttonCodeMap;
	buttonCodeMap['w'] = ButtonEvent::UP;
	buttonCodeMap['a'] = ButtonEvent::LEFT;
	buttonCodeMap['s'] = ButtonEvent::DOWN;
	buttonCodeMap['d'] = ButtonEvent::RIGHT;
	buttonCodeMap['q'] = ButtonEvent::A;
	buttonCodeMap['e'] = ButtonEvent::B;

	// Note: it appears that it is not legal to call the ncurses getch() function
	// in this thread but execute the ncurses output functions in another thread.
	// So, we must read directly from stdin.  This actually isn't a great hardship:
	// the only issue will be decoding escape sequences (e.g., for the arrow keys.)
	
	enum {
		NORMAL,
		ESC_SEEN,
		LBRACKET_SEEN,
	} state = NORMAL;
	
	for (;;) {
		int ch;
		ch = fgetc(stdin);
		if (ch < 0) {
			break;
		}
		
		switch (state) {
			case NORMAL:
				if (ch == 27) {
					state = ESC_SEEN;
				} else {
					ButtonCodeMap::iterator i = buttonCodeMap.find(ch);
					if (i != buttonCodeMap.end()) {
						EventQueue::instance()->enqueue(new ButtonEvent(ButtonEvent::RELEASE, i->second));
					}
				}
				break;
			
			case ESC_SEEN:
				state = (ch == '[') ? LBRACKET_SEEN : NORMAL;
				break;
			
			case LBRACKET_SEEN:
				switch (ch) {
					case 'A':
						EventQueue::instance()->enqueue(new ButtonEvent(ButtonEvent::RELEASE, ButtonEvent::UP));
						break;
					case 'B':
						EventQueue::instance()->enqueue(new ButtonEvent(ButtonEvent::RELEASE, ButtonEvent::DOWN));
						break;
					case 'C':
						EventQueue::instance()->enqueue(new ButtonEvent(ButtonEvent::RELEASE, ButtonEvent::RIGHT));
						break;
					case 'D':
						EventQueue::instance()->enqueue(new ButtonEvent(ButtonEvent::RELEASE, ButtonEvent::LEFT));
						break;
				}
				state = NORMAL;
				break;
		}
	}
}
