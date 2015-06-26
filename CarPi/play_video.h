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

#ifndef PLAYVIDEO_H
#define PLAYVIDEO_H

#include <string>
#include <sys/types.h>

class Thread;
class PlaybackSettings;

//
// Play video using omxplayer.
//
class PlayVideo
{
public:
	enum State {
		IDLE,
		PLAYING,
		PAUSED,
		EXITING,
	};

	enum SeekDir {
		BACKWARD,
		FORWARD,
	};

	enum SeekSize {
		SMALL,
		LARGE,
	};

private:
	State m_state;
	std::string m_fileName;
	pid_t m_pid;
	int m_cmdfd; // send commands to omxplayer
	int m_statusfd; // read status from omxplayer subprocess stdout
	int m_errfd; // read from subprocess  omxplayer stderr
	Thread *m_stdoutMonitor;
	Thread *m_stderrMonitor;
	PlaybackSettings *m_playbackSettings;
	
public:
	PlayVideo(const std::string &fileName);
	~PlayVideo();

	std::string getFileName() const { return m_fileName; }
	State getState() const { return m_state; }

	bool play();
	bool seek(SeekDir dir, SeekSize size);
	bool pause();
	bool stop();
	bool waitForIdle();

	const PlaybackSettings *getPlaybackSettings() {
		return m_playbackSettings;
	}
	bool updatePlaybackSettings(const PlaybackSettings *other);
	
private:
	bool startProcess();
	void sendCommand(const std::string &cmd);
	static void closefd(int fd);
};

#endif // PLAYVIDEO_H
