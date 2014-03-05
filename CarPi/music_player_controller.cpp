#include "car_pi_app.h"
#include "event.h"
#include "event_queue.h"
#include "playback_settings.h"
#include "play_sound.h"
#include "music_player_controller.h"

namespace {
	//
	// Callback to receive status updates from the player subprocess
	// and post them to the event queue (so the view can respond to them
	// appropriately.)
	//
	class StatusUpdateCallback : public PlaySoundCallback {
	private:
		PlaySound *m_playSound;
		
	public:
		StatusUpdateCallback(PlaySound *m_playSound);
		virtual ~StatusUpdateCallback();
		
		virtual void onFileName(const std::string &fileName);
		virtual void onID3(const std::string &title, const std::string &artist, const std::string &album);
		virtual void onFrame(int curFrame, int remainingFrames, float curTime, float remainingTime);
		virtual void onPlayStatus(PlayStatus playStatus);
	};
	
	StatusUpdateCallback::StatusUpdateCallback(PlaySound *playSound)
		: m_playSound(playSound)
	{
	}
	
	StatusUpdateCallback::~StatusUpdateCallback()
	{
	}

	void StatusUpdateCallback::onFileName(const std::string &fileName)
	{
		EventQueue::instance()->enqueue(new MediaFileInfoEvent(fileName));
	}
	
	void StatusUpdateCallback::onID3(const std::string &title, const std::string &artist, const std::string &album)
	{
		EventQueue::instance()->enqueue(new MediaFileInfoEvent(title, artist, album));
	}
	
	void StatusUpdateCallback::onFrame(int curFrame, int remainingFrames, float curTime, float remainingTime)
	{
		EventQueue::instance()->enqueue(new MediaStatusEvent(curFrame, remainingFrames, curTime, remainingTime));
	}
	
	void StatusUpdateCallback::onPlayStatus(PlayStatus playStatus)
	{
		switch (playStatus) {
			case STOPPED:
				EventQueue::instance()->enqueue(new NotificationEvent(NotificationEvent::PLAYER_STOPPED, m_playSound));
				break;
			case PAUSED:
				EventQueue::instance()->enqueue(new NotificationEvent(NotificationEvent::PLAYER_PAUSED, m_playSound));
				break;
			case PLAYING:
				EventQueue::instance()->enqueue(new NotificationEvent(NotificationEvent::PLAYER_PLAYING, m_playSound));
				break;
			case ENDED:
				EventQueue::instance()->enqueue(new NotificationEvent(NotificationEvent::PLAYER_ENDED, m_playSound));
				break;
		}
	}
}

MusicPlayerController::MusicPlayerController(PlaySound *playSound)
	: m_playSound(playSound)
	, m_callback(new StatusUpdateCallback(playSound))
{
	m_playSound->setCallback(m_callback);
}

MusicPlayerController::~MusicPlayerController()
{
	if (m_playSound != 0) {
		m_playSound->stop();
		m_playSound->waitForIdle();
		delete m_playSound;
		delete m_callback;
	}
}

void MusicPlayerController::visitButtonEvent(ButtonEvent *evt)
{
	if (evt->getType() == ButtonEvent::RELEASE) {
		switch (evt->getCode()) {
			case ButtonEvent::LEFT:
				// Go back
				setResult(EventHandler::HANDLED);
				CarPiApp::instance()->popEventHandler();
				break;
			case ButtonEvent::UP:
				// Go to previous file
				setResult(EventHandler::HANDLED);
				m_playSound->previous();
				break;
			case ButtonEvent::DOWN:
				// Go to next file
				setResult(EventHandler::HANDLED);
				m_playSound->next();
				break;
			case ButtonEvent::A:
				// pause or resume
				if (m_playSound->getState() == PlaySound::PAUSED) {
					m_playSound->resume();
				} else if (m_playSound->getState() == PlaySound::PLAYING) {
					m_playSound->pause();
				}
				break;
			case ButtonEvent::B:
				// playback settings
				CarPiApp::instance()->startPlaybackSettingsEditor(this, m_playSound->getPlaybackSettings()->clone());
				break;
			default:
				break;
		}
	}
}

void MusicPlayerController::visitNotificationEvent(NotificationEvent *evt)
{
	// Listen for PLAYER_ENDED events: if there are more files to
	// play, start the next one.
	NotificationEvent::Type type = evt->getType();
	if (type == NotificationEvent::PLAYER_ENDED) {
		size_t selected = m_playSound->getSelectedFile();
		if (m_playSound->getNumFiles() > 1 && selected < m_playSound->getNumFiles() - 1) {
			m_playSound->play(selected + 1);
		}
		// NOTE: don't set the result to HANDLED, since the view might also
		// want to handle this notification
	} else if (type == NotificationEvent::PLAYBACK_SETTINGS_CHANGED) {
		const PlaybackSettings *playbackSettings =
			static_cast<const PlaybackSettings *>(evt->getObject());
		m_playSound->updatePlaybackSettings(playbackSettings);
		// Don't set result to HANDLED: there is almost certainly
		// another controller/view pair active (the playback settings
		// controller/view)
	}
}
