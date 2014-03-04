#ifndef PLAYBACK_SETTINGS_CONTROLLER_H
#define PLAYBACK_SETTINGS_CONTROLLER_H

#include "abstract_event_visitor.h"

class PlaybackSettings;

class PlaybackSettingsController : public AbstractEventVisitor {
private:
	PlaybackSettings *m_playbackSettings;

public:
	PlaybackSettingsController(PlaybackSettings *playbackSettingsToAdopt);
	virtual ~PlaybackSettingsController();

	virtual void visitButtonEvent(ButtonEvent *evt);

private:
	void changeVolume(int delta);
};


#endif // PLAYBACK_SETTINGS_CONTROLLER_H
