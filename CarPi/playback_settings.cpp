// CarPi - Raspberry Pi car entertainment system
// Copyright (c) 2013-2015 David H. Hovemeyer <david.hovemeyer@gmail.com>

// This file is part of CarPi.
// 
// CarPi is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
// 
// CarPi is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with CarPi.  If not, see <http://www.gnu.org/licenses/>.

#include "playback_settings.h"

PlaybackSettings::PlaybackSettings()
	: m_volumePercent(100)
	, m_outputType(COMPOSITE)
{
}

PlaybackSettings::PlaybackSettings(const PlaybackSettings& other)
	: m_volumePercent(other.m_volumePercent)
	, m_outputType(other.m_outputType)
{
}

PlaybackSettings::~PlaybackSettings()
{
}

PlaybackSettings &PlaybackSettings::operator=(const PlaybackSettings &rhs)
{
	if (this != &rhs) {
		m_volumePercent = rhs.m_volumePercent;
		m_outputType = rhs.m_outputType;
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

void PlaybackSettings::setOutputType(OutputType outputType)
{
	m_outputType = outputType;
}

PlaybackSettings::OutputType PlaybackSettings::getOutputType() const
{
	return m_outputType;
}

PlaybackSettings *PlaybackSettings::clone() const
{
	return new PlaybackSettings(*this);
}

bool PlaybackSettings::operator==(const PlaybackSettings &rhs) const {
	return this->m_volumePercent == rhs.m_volumePercent
		&& this->m_outputType == rhs.m_outputType;
}
