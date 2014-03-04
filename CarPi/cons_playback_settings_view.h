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
