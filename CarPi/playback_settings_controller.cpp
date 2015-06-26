// CarPi - Raspberry Pi car entertainment system
// Copyright (c) 2013-2015 David H. Hovemeyer <david.hovemeyer@gmail.com>

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

#include "event_queue.h"
#include "playback_settings.h"
#include "car_pi_app.h"
#include "playback_settings_controller.h"

PlaybackSettingsController::PlaybackSettingsController(PlaybackSettings *playbackSettingsToAdopt)
	: m_playbackSettings(playbackSettingsToAdopt)
{
}

PlaybackSettingsController::~PlaybackSettingsController()
{
	delete m_playbackSettings;
}

void PlaybackSettingsController::visitButtonEvent(ButtonEvent *evt)
{
	if (evt->getType() == ButtonEvent::RELEASE) {
		switch (evt->getCode()) {
		case ButtonEvent::LEFT:
			// Decrease volume
			setResult(EventHandler::HANDLED);
			changeVolume(-2);
			break;

		case ButtonEvent::RIGHT:
			// Increase volume
			setResult(EventHandler::HANDLED);
			changeVolume(2);
			break;

		case ButtonEvent::B:
			// Go back to parent view
			setResult(EventHandler::HANDLED);
			CarPiApp::instance()->popEventHandler();
			break;

		case ButtonEvent::A:
			// toggle between composite and HDMI video output
			setResult(EventHandler::HANDLED);
			toggleOutputType();
			break;

		default:
			break;
		}
	}
}

void PlaybackSettingsController::changeVolume(int delta)
{
	int volume = m_playbackSettings->getVolumePercent();
	volume += delta;
	if (volume < 0) {
		volume = 0;
	} else if (volume > 100) {
		volume = 100;
	}
	m_playbackSettings->setVolumePercent(volume);
	EventQueue::instance()->enqueue(
		new NotificationEvent(NotificationEvent::PLAYBACK_SETTINGS_CHANGED, m_playbackSettings)
	);
}

void PlaybackSettingsController::toggleOutputType()
{
	m_playbackSettings->setOutputType(
		(m_playbackSettings->getOutputType() == PlaybackSettings::COMPOSITE)
			? PlaybackSettings::HDMI
			: PlaybackSettings::COMPOSITE
	);
	EventQueue::instance()->enqueue(
		new NotificationEvent(NotificationEvent::PLAYBACK_SETTINGS_CHANGED, m_playbackSettings)
	);
}
