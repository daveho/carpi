#include "console.h"
#include "cons_music_player_view.h"

ConsMusicPlayerView::ConsMusicPlayerView(const PlaySound *playSound)
	: m_playSound(playSound)
	, m_playerStatus(UNKNOWN)
	, m_curTime(0)
	, m_totalTime(0)
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
			
		case NotificationEvent::PLAYER_PLAYING:
			setResult(EventHandler::HANDLED);
			m_playerStatus = PLAYING;
			doPaint();
			
		default:
			break;
	}
}

void ConsMusicPlayerView::visitMediaFileInfoEvent(MediaFileInfoEvent *evt)
{
	setResult(EventHandler::HANDLED);
	m_title = evt->getTitle();
	m_artist = evt->getArtist();
	m_album = evt->getAlbum();
	doPaint();
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
	
	size_t w = size_t(cons->getNumCols());
	
	cons->clear();
	
	cons->attr(Console::BLACK, Console::CYAN+Console::INTENSE);
	cons->moveCursor(2, 0);
	cons->print(m_title.substr(0, w));
	
	cons->attr(Console::BLACK, Console::CYAN);
	cons->moveCursor(4, 0);
	cons->print(m_artist.substr(0, w));
	
	cons->attr(Console::BLACK, Console::BLUE);
	cons->moveCursor(6, 0);
	cons->print(m_album.substr(0, w));
	
	cons->attr(Console::BLACK, Console::GRAY);
	
	cons->moveCursor(8, 0);
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
	
	cons->moveCursor(10, 0);
	//cons->printf("%.1f / %.1f", m_curTime, m_totalTime); // TODO: better formatting
	int curTimeSec = m_curTime / 100;
	int totalTimeSec = m_totalTime / 100;
	cons->printf("% 2d:%02d / % 2d:%02d", curTimeSec/60, curTimeSec%60, totalTimeSec/60, totalTimeSec%60);
	
	cons->commit();
}
