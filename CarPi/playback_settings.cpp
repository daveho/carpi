#include "playback_settings.h"

PlaybackSettings::PlaybackSettings()
	: m_volumePercent(100)
{
}

PlaybackSettings::PlaybackSettings(const PlaybackSettings& other)
	: m_volumePercent(other.m_volumePercent)
{
}

PlaybackSettings::~PlaybackSettings()
{
}

PlaybackSettings &PlaybackSettings::operator=(const PlaybackSettings &rhs)
{
	if (this != &rhs) {
		m_volumePercent = rhs.m_volumePercent;
	}
	return *this;
}

void PlaybackSettings::setVolumePercent(int volumePercent)
{
	m_volumePercent = volumePercent;
}

int PlaybackSettings::getVolumePercent() const
{
	return m_volumePercent;
}

PlaybackSettings *PlaybackSettings::clone() const
{
	return new PlaybackSettings(*this);
}

bool PlaybackSettings::operator==(const PlaybackSettings &rhs) const {
	return this->m_volumePercent == rhs.m_volumePercent;
}
