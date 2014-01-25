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

#ifndef CONSMUSICPLAYERVIEW_H
#define CONSMUSICPLAYERVIEW_H

#include "abstract_event_visitor.h"
#include "event.h"

class PlaySound;

class ConsMusicPlayerView : public AbstractEventVisitor
{
private:
	enum PlayerStatus {
		UNKNOWN,
		PLAYING,
		PAUSED,
		STOPPED,
	};

	const PlaySound *m_playSound;
	PlayerStatus m_playerStatus;
	std::string m_fileName;
	std::string m_title;
	std::string m_artist;
	std::string m_album;
	int m_curTime, m_totalTime;
	
public:
	ConsMusicPlayerView(const PlaySound *playSound);
	~ConsMusicPlayerView();

	virtual void visitNotificationEvent(NotificationEvent *evt);
	virtual void visitMediaFileInfoEvent(MediaFileInfoEvent *evt);
	virtual void visitMediaStatusEvent(MediaStatusEvent *evt);
	
private:
	void doPaint();
};

#endif // CONSMUSICPLAYERVIEW_H
