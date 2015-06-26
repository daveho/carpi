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

#include "car_pi_app.h"
#include "event_queue.h"
#include "play_video.h"
#include "playback_settings.h"
#include "video_player_controller.h"

VideoPlayerController::VideoPlayerController(PlayVideo *playVideo)
	: m_playVideo(playVideo)
	, m_rightPressed(false)
{
}

VideoPlayerController::~VideoPlayerController()
{
}

void VideoPlayerController::visitButtonEvent(ButtonEvent *evt)
{
	if (evt->getType() == ButtonEvent::RELEASE) {
		switch (evt->getCode()) {
			case ButtonEvent::A:
				setResult(EventHandler::HANDLED);
				if (m_playVideo->getState() == PlayVideo::IDLE
					|| m_playVideo->getState() == PlayVideo::PAUSED) {
					// Stopped/paused -> Playing
					m_playVideo->play();
					EventQueue::instance()->repaint();
				} else if (m_playVideo->getState() == PlayVideo::PLAYING) {
					// Playing -> Paused
					m_playVideo->pause();
					EventQueue::instance()->repaint();
				}
				break;

			case ButtonEvent::LEFT:
				setResult(EventHandler::HANDLED);

				// Ignore this button press if the video is playing:
				// it must be paused first.  We want to prevent
				// accidental stoppages (which would upset the
				// viewers :-)
				if (m_playVideo->getState() == PlayVideo::PLAYING) {
					break;
				}

				// If the omxplayer subprocess has started, send the
				// quit command and wait for it to stop.
				if (m_playVideo->getState() > PlayVideo::IDLE) {
					// Shutdown
					m_playVideo->stop();
					m_playVideo->waitForIdle();
				}

				// Go back to the video navigation menu.
				CarPiApp::instance()->popEventHandler();

				break;

			case ButtonEvent::RIGHT:
				// The right arrow acts as a modifier for the up
				// (seek backwards) and down (seek forwards)
				// buttons.
				setResult(EventHandler::HANDLED);
				m_rightPressed = true;
				break;

			case ButtonEvent::UP:
				// Seek backwards
				setResult(EventHandler::HANDLED);
				m_playVideo->seek(
					PlayVideo::BACKWARD,
					m_rightPressed ? PlayVideo::LARGE : PlayVideo::SMALL);
				m_rightPressed = false;
				break;

			case ButtonEvent::DOWN:
				// Seek forwards
				setResult(EventHandler::HANDLED);
				m_playVideo->seek(
					PlayVideo::FORWARD,
					m_rightPressed ? PlayVideo::LARGE : PlayVideo::SMALL);
				m_rightPressed = false;
				break;

			case ButtonEvent::B:
				// playback settings
				CarPiApp::instance()->startPlaybackSettingsEditor(this, m_playVideo->getPlaybackSettings()->clone());
				break;

			default:
				break;
		}
	}
}

void VideoPlayerController::visitNotificationEvent(NotificationEvent *evt)
{
	NotificationEvent::Type type = evt->getType();

	if (type == NotificationEvent::PLAYBACK_SETTINGS_CHANGED) {
		const PlaybackSettings *playbackSettings =
			static_cast<const PlaybackSettings *>(evt->getObject());
		m_playVideo->updatePlaybackSettings(playbackSettings);
	}
}
