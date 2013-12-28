#ifndef CONSMUSICPLAYERVIEW_H
#define CONSMUSICPLAYERVIEW_H

#include "abstract_event_visitor.h"
#include "event.h"

class PlaySound;

class ConsMusicPlayerView : public AbstractEventVisitor
{
private:
	enum PlayerStatus {
		UNKNOWN,
		PLAYING,
		PAUSED,
		STOPPED,
	};

	const PlaySound *m_playSound;
	PlayerStatus m_playerStatus;
	std::string m_title;
	std::string m_artist;
	std::string m_album;
	float m_curTime, m_totalTime;
	
public:
	ConsMusicPlayerView(const PlaySound *playSound);
	~ConsMusicPlayerView();

	virtual void visitNotificationEvent(NotificationEvent *evt);
	virtual void visitMediaFileInfoEvent(MediaFileInfoEvent *evt);
	virtual void visitMediaStatusEvent(MediaStatusEvent *evt);
	
private:
	void doPaint();
};

#endif // CONSMUSICPLAYERVIEW_H
