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
#include "playback_settings.h"
#include "cons_playback_settings_view.h"

ConsPlaybackSettingsView::ConsPlaybackSettingsView(PlaybackSettings *playbackSettings)
	: m_playbackSettings(playbackSettings)
{
}

ConsPlaybackSettingsView::~ConsPlaybackSettingsView()
{
	// the controller has responsibility for deleting the PlaybackSettings
}

void ConsPlaybackSettingsView::visitNotificationEvent(NotificationEvent *evt)
{
	switch (evt->getType()) {
	case NotificationEvent::PAINT:
	case NotificationEvent::PLAYBACK_SETTINGS_CHANGED:
		setResult(EventHandler::HANDLED);
		onPaint();
		break;

	default:
		break;
	}
}

void ConsPlaybackSettingsView::onPaint()
{
	Console *cons = Console::instance();

	cons->clear();
	cons->moveCursor(2, 0);
	cons->printf("Volume: %3d%%", m_playbackSettings->getVolumePercent());
	cons->moveCursor(cons->getNumRows() - 2, 0);
	cons->commit();
}
