#include "cons_music_player_view.h"

ConsMusicPlayerView::ConsMusicPlayerView(const PlaySound *playSound)
	: m_playSound(playSound)
{
}

ConsMusicPlayerView::~ConsMusicPlayerView()
{
}

void ConsMusicPlayerView::visitNotificationEvent(NotificationEvent *evt)
{
	
}
