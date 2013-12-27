#include "console.h"
#include "cons_music_player_view.h"

ConsMusicPlayerView::ConsMusicPlayerView(const PlaySound *playSound)
	: m_playSound(playSound)
	, m_playerStatus(UNKNOWN)
	, m_curTime(0.0f)
	, m_totalTime(0.0f)
{
}

ConsMusicPlayerView::~ConsMusicPlayerView()
{
	// Note: the controller takes care of de-allocating the PlaySound object
}

void ConsMusicPlayerView::visitNotificationEvent(NotificationEvent *evt)
{
	switch (evt->getType()) {
		case NotificationEvent::PAINT:
			setResult(EventHandler::HANDLED);
			doPaint();
			break;
			
		case NotificationEvent::PLAYER_STOPPED:
		case NotificationEvent::PLAYER_ENDED:
			setResult(EventHandler::HANDLED);
			m_playerStatus = STOPPED;
			doPaint();
			break;
			
		case NotificationEvent::PLAYER_PAUSED:
			setResult(EventHandler::HANDLED);
			m_playerStatus = PAUSED;
			doPaint();
			
		case NotificationEvent::PLAYER_RESUMED:
			setResult(EventHandler::HANDLED);
			m_playerStatus = PLAYING;
			doPaint();
			
		default:
			break;
	}
}

void ConsMusicPlayerView::visitMediaStatusEvent(MediaStatusEvent *evt)
{
	setResult(EventHandler::HANDLED);
	m_curTime = evt->getCurTime();
	m_totalTime = evt->getCurTime() + evt->getRemainingTime();
	doPaint();
}

void ConsMusicPlayerView::doPaint()
{
	Console *cons = Console::instance();
	
	cons->clear();
	
	cons->attr(Console::BLACK, Console::GRAY);
	
	cons->moveCursor(2, 0);
	switch (m_playerStatus) {
		case PLAYING:
			cons->print("Playing"); break;
		case PAUSED:
			cons->print("Paused"); break;
		case STOPPED:
			cons->print("Stopped"); break;
		default:
			break;
	}
	
	cons->moveCursor(4, 0);
	cons->printf("%.1f / %.1f", m_curTime, m_totalTime); // TODO: better formatting
	
	cons->commit();
}
