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

#ifndef PLAYBACK_SETTINGS
#define PLAYBACK_SETTINGS

class PlaybackSettings {
public:
	enum OutputType {
		COMPOSITE,
		HDMI,
	};

private:
	int m_volumePercent;
	OutputType m_outputType;

public:
	PlaybackSettings();
	PlaybackSettings(const PlaybackSettings& other);
	virtual ~PlaybackSettings();

	PlaybackSettings &operator=(const PlaybackSettings &rhs);

	void setVolumePercent(int volumePercent);
	int getVolumePercent() const;

	void setOutputType(OutputType outputType);
	OutputType getOutputType() const;

	PlaybackSettings *clone() const;

	bool operator==(const PlaybackSettings &rhs) const;
	bool operator!=(const PlaybackSettings &rhs) const { return !(*this == rhs); }
};

#endif // PLAYBACK_SETTINGS
