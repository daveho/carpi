#include "notification_event_filter.h"

NotificationEventFilter::NotificationEventFilter(EventHandler *delegate)
	: m_delegate(delegate)
{
}

NotificationEventFilter::~NotificationEventFilter()
{
	// Note that the delegate is NOT deleted
}

void NotificationEventFilter::visitNotificationEvent(NotificationEvent *evt)
{
	if (m_delegate->handleEvent(evt) == EventHandler::HANDLED) {
		setResult(EventHandler::HANDLED);
	}
}
