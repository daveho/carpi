#ifndef NOTIFICATION_EVENT_FILTER_H
#define NOTIFICATION_EVENT_FILTER_H

#include "abstract_event_visitor.h"

//
// Delegating event handler that only sends notification
// events to the delegate.  We use this for cases such as
// editing playback settings, where the "parent" controller
// (e.g., music player controller) should receive notifications
// of event settings changes, even though a different
// controller/view are active.
//
class NotificationEventFilter : public AbstractEventVisitor {
private:
	EventHandler *m_delegate;

public:
	NotificationEventFilter(EventHandler *delegate);
	virtual ~NotificationEventFilter();

	virtual void visitNotificationEvent(NotificationEvent *evt);
};

#endif // NOTIFICATION_EVENT_FILTER_H
