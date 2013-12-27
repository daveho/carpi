#ifndef CONSMUSICPLAYERVIEW_H
#define CONSMUSICPLAYERVIEW_H

#include "abstract_event_visitor.h"

class PlaySound;

class ConsMusicPlayerView : public AbstractEventVisitor
{
private:
	const PlaySound *m_playSound;
	
public:
	ConsMusicPlayerView(const PlaySound *playSound);
	~ConsMusicPlayerView();

	virtual void visitNotificationEvent(NotificationEvent *evt);
};

#endif // CONSMUSICPLAYERVIEW_H
