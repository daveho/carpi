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

#include "console.h"
#include "string_util.h"
#include "play_video.h"
#include "cons_video_player_view.h"

ConsVideoPlayerView::ConsVideoPlayerView(PlayVideo *playVideo)
	: m_playVideo(playVideo)
{
}

ConsVideoPlayerView::~ConsVideoPlayerView()
{
}

void ConsVideoPlayerView::visitNotificationEvent(NotificationEvent *evt)
{
	if (evt->getType() == NotificationEvent::PAINT) {
		setResult(EventHandler::HANDLED);
		onPaint();
	}
}

void ConsVideoPlayerView::onPaint()
{
	// This is pretty basic for now
	Console *cons = Console::instance();

	cons->clear();
	cons->attr(Console::BLACK, Console::GRAY);

	std::string filePart = StringUtil::getFilePart(m_playVideo->getFileName());
	cons->printCenter(2, filePart);

	switch (m_playVideo->getState()) {
		case PlayVideo::IDLE:
		case PlayVideo::EXITING:
			cons->printCenter(4, "Stopped"); break;
		case PlayVideo::PLAYING:
			cons->printCenter(4, "Playing"); break;
		case PlayVideo::PAUSED:
			cons->printCenter(4, "Paused"); break;
	}

	cons->commit();
}
