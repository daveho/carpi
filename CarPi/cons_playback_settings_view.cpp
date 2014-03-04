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
