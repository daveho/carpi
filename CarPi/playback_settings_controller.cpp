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
			changeVolume(-2);
			break;

		case ButtonEvent::RIGHT:
			// Increase volume
			changeVolume(2);
			break;

		case ButtonEvent::B:
			// Go back to parent view
			CarPiApp::instance()->popEventHandler();
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
