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

#ifndef CONS_PLAYBACK_SETTINGS_VIEW_H
#define CONS_PLAYBACK_SETTINGS_VIEW_H

#include "abstract_event_visitor.h"

class PlaybackSettings;

class ConsPlaybackSettingsView : public AbstractEventVisitor {
private:
	PlaybackSettings *m_playbackSettings;

public:
	ConsPlaybackSettingsView(PlaybackSettings *playbackSettings);
	virtual ~ConsPlaybackSettingsView();

	virtual void visitNotificationEvent(NotificationEvent *evt);

private:
	void onPaint();
};


#endif // CONS_PLAYBACK_SETTINGS_VIEW_H
