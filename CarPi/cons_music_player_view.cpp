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

#include <cstdio>
#include "console.h"
#include "play_sound.h"
#include "string_util.h"
#include "cons_music_player_view.h"

namespace {
	int printLines(const std::vector<std::string> &lines, int row, int maxLines, Console *cons) {
		std::vector<std::string>::const_iterator i = lines.begin();
		int numOut = 0;
		while (i != lines.end() && numOut < maxLines) {
			cons->printCenter(row, *i);
			i++;
			numOut++;
			row++;
		}
		return row;
	}
}

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
	
	cons->attr(Console::BLACK, Console::GRAY);
	cons->moveCursor(0, 0);
	cons->printf("%d", cons->getNumCols());

	int row = 2;
	
	if (!m_fileName.empty()) {
		cons->attr(Console::BLACK, Console::CYAN+Console::INTENSE);
		cons->printCenter(row, m_fileName);
		row += 2;
	} else {
		std::vector<std::string> lines;
		
		StringUtil::splitIntoLines(m_title, size_t(cons->getNumCols()), lines);
		cons->attr(Console::BLACK, Console::CYAN+Console::INTENSE);
		row = printLines(lines, row, 3, cons);
		row++;

		StringUtil::splitIntoLines(m_artist, size_t(cons->getNumCols()), lines);
		cons->attr(Console::BLACK, Console::CYAN);
		row = printLines(lines, row, 3, cons);
		row++;
		
		StringUtil::splitIntoLines(m_album, size_t(cons->getNumCols()), lines);
		cons->attr(Console::BLACK, Console::BLUE);
		row = printLines(lines, row, 3, cons);
		row += 2;
	}
		
	cons->attr(Console::BLACK, Console::GRAY);
	
	switch (m_playerStatus) {
		case PLAYING:
			cons->printCenter(row, "Playing"); break;
		case PAUSED:
			cons->printCenter(row, "Paused"); break;
		case STOPPED:
			cons->printCenter(row, "Stopped"); break;
		default:
			break;
	}
	row ++;
	
	int curTimeSec = m_curTime / 100;
	int totalTimeSec = m_totalTime / 100;
	cons->moveCursor(row, (cons->getNumCols() - 13) / 2);
	cons->printf("% 2d:%02d / % 2d:%02d", curTimeSec/60, curTimeSec%60, totalTimeSec/60, totalTimeSec%60);
	row++;
	
	if (m_playSound->getNumFiles() > 1) {
		char *buf = new char[100];
		sprintf(buf, "Track %d / %d", int(m_playSound->getSelectedFile())+1, int(m_playSound->getNumFiles()));
		cons->printCenter(row, buf);
		delete[] buf;
	}
	
	cons->commit();
}
