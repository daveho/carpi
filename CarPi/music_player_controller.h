#ifndef MUSICPLAYERCONTROLLER_H
#define MUSICPLAYERCONTROLLER_H

#include <vector>
#include <string>
#include "abstract_event_visitor.h"

class PlaySound;
class PlaySoundCallback;

class MusicPlayerController : public AbstractEventVisitor
{
private:
	typedef std::vector<std::string> FileList;
	FileList m_fileList;
	PlaySound *m_playSound;
	PlaySoundCallback *m_callback;
	
public:
	MusicPlayerController(PlaySound *playSound);
	~MusicPlayerController();

	virtual void visitButtonEvent(ButtonEvent *evt);
	virtual void visitNotificationEvent(NotificationEvent *evt);
};

#endif // MUSICPLAYERCONTROLLER_H
