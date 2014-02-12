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
#include "console.h"
#include "play_sound.h"
#include "cons_music_player_view.h"

ConsMusicPlayerView::ConsMusicPlayerView(const PlaySound *playSound)
	: m_playSound(playSound)
	, m_playerStatus(UNKNOWN)
	, m_curTime(0)
	, m_totalTime(0)
{
}

ConsMusicPlayerView::~ConsMusicPlayerView()
{
	// Note: the controller takes care of de-allocating the PlaySound object
}

void ConsMusicPlayerView::visitNotificationEvent(NotificationEvent *evt)
{
	switch (evt->getType()) {
		case NotificationEvent::PAINT:
			setResult(EventHandler::HANDLED);
			doPaint();
			break;
			
		case NotificationEvent::PLAYER_STOPPED:
		case NotificationEvent::PLAYER_ENDED:
			setResult(EventHandler::HANDLED);
			m_playerStatus = STOPPED;
			doPaint();
			break;
			
		case NotificationEvent::PLAYER_PAUSED:
			setResult(EventHandler::HANDLED);
			m_playerStatus = PAUSED;
			doPaint();
			
		case NotificationEvent::PLAYER_PLAYING:
			setResult(EventHandler::HANDLED);
			m_playerStatus = PLAYING;
			doPaint();
			
		default:
			break;
	}
}

void ConsMusicPlayerView::visitMediaFileInfoEvent(MediaFileInfoEvent *evt)
{
	setResult(EventHandler::HANDLED);
	if (evt->hasMetadata()) {
		m_title = evt->getTitle();
		m_artist = evt->getArtist();
		m_album = evt->getAlbum();
	} else {
		m_fileName = evt->getFileName();
	}
	doPaint();
}

void ConsMusicPlayerView::visitMediaStatusEvent(MediaStatusEvent *evt)
{
	setResult(EventHandler::HANDLED);
	m_curTime = evt->getCurTime();
	m_totalTime = evt->getCurTime() + evt->getRemainingTime();
	doPaint();
}

void ConsMusicPlayerView::doPaint()
{
	Console *cons = Console::instance();
	
	cons->clear();
	
	if (!m_fileName.empty()) {
		cons->attr(Console::BLACK, Console::GRAY+Console::INTENSE);
		cons->printCenter(2, m_fileName);
	} else {
		cons->attr(Console::BLACK, Console::GRAY+Console::INTENSE);
		cons->printCenter(2, m_title);
		
		cons->attr(Console::BLACK, Console::CYAN);
		cons->printCenter(4, m_artist);
		
		cons->attr(Console::BLACK, Console::GREEN+Console::INTENSE);
		cons->printCenter(6, m_album);
	}
		
	cons->attr(Console::BLACK, Console::GRAY);
	
	switch (m_playerStatus) {
		case PLAYING:
			cons->printCenter(8, "Playing"); break;
		case PAUSED:
			cons->printCenter(8, "Paused"); break;
		case STOPPED:
			cons->printCenter(8, "Stopped"); break;
		default:
			break;
	}
	
	int curTimeSec = m_curTime / 100;
	int totalTimeSec = m_totalTime / 100;
	cons->moveCursor(10, (cons->getNumCols() - 13) / 2);
	cons->printf("% 2d:%02d / % 2d:%02d", curTimeSec/60, curTimeSec%60, totalTimeSec/60, totalTimeSec%60);
	
	if (m_playSound->getNumFiles() > 1) {
		char *buf = new char[100];
		sprintf(buf, "Track %d / %d", int(m_playSound->getSelectedFile())+1, int(m_playSound->getNumFiles()));
		cons->printCenter(11, buf);
		delete[] buf;
	}

	// move the cursor to the bottom of the screen,
	// where it will be less distracting
	cons->moveCursor(cons->getNumRows() - 2, 0);
	
	cons->commit();
}
