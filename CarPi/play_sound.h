// CarPi - Raspberry Pi car entertainment system
// Copyright (c) 2013, David H. Hovemeyer <david.hovemeyer@gmail.com>

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

#ifndef PLAYSOUND_H
#define PLAYSOUND_H

#include <cstddef>
#include <vector>
#include <string>
#include <sys/types.h>

#include "thread.h"

class PlaybackSettings;

//
// Callback interface to receive asynchronous status updates
// from the PlaySound subprocess.
//
class PlaySoundCallback {
public:
	enum PlayStatus {
		STOPPED = 0,
		PAUSED = 1,
		PLAYING = 2, // equivalent to resumed
		ENDED = 3,
	};

	PlaySoundCallback();
	virtual ~PlaySoundCallback();
	
	virtual void onFileName(const std::string &fileName) = 0;
	virtual void onID3(const std::string &title, const std::string &artist, const std::string &album) = 0;
	virtual void onFrame(int curFrame, int remainingFrames, float curTime, float remainingTime) = 0;
	virtual void onPlayStatus(PlayStatus playStatus) = 0;
};

//
// Play a sound file using mpg321 or ogg123 as a subprocess,
// using their remote control interface.
//
class PlaySound
{
public:
	//
	// "Synchronous" player state: these are only affected
	// by calls to the control methods, not by the asynchronous
	// state of the subprocess.  Subprocess state must
	// be monitored using the status callback (whose methods
	// will be called from the subprocess monitor thread.)
	//
	enum State {
		IDLE,     // subprocess not running
		ACTIVE,   // subprocess is running, but playing has not started
		PLAYING,  // subprocess is playing sound file
		PAUSED,   // subprocess is paused
		EXITING,  // subprocess is exiting
	};
	
	enum FileType {
		UNKNOWN,
		MP3,
		OGG,
	};
	
private:
	//
	// Monitor thread class
	//
	class MonitorThread : public Thread {
	private:
		PlaySound *m_ps;
		PlaySound::FileType m_fileType;
		PlaySoundCallback::PlayStatus m_currentStatus;
		
	public:
		MonitorThread(PlaySound *ps, FileType fileType);
		virtual ~MonitorThread();
		virtual void run();
		
	private:
		void updateStatus(PlaySoundCallback::PlayStatus status);
		static void parseId3(const std::string &line, std::string &title, std::string &artist, std::string &album);
		static int parseHundredths(const std::string &s);
	};

	typedef std::vector<std::string> FileList;
	FileList m_fileList;
	State m_state;
	size_t m_selectedFile;
	pid_t m_pid;
	int m_cmdfd;    // pipe to send commands to subprocess
	int m_statusfd; // pipe to receive status updates from subprocess
	MonitorThread *m_monitor;
	PlaySoundCallback *m_callback;
	PlaybackSettings *m_playbackSettings;

public:
	PlaySound();
	~PlaySound();
	
	void addFile(const std::string &fileName);
	size_t getNumFiles() const { return m_fileList.size(); }
	
	void setCallback(PlaySoundCallback *callback) { m_callback = callback; }

	State getState() const { return m_state; }
	size_t getSelectedFile() const { return m_selectedFile; }

	const PlaybackSettings *getPlaybackSettings() {
		return m_playbackSettings;
	}
	
	bool play(size_t i);
	bool previous();
	bool next();
	bool pause();
	bool resume();
	bool stop();
	bool updatePlaybackSettings(const PlaybackSettings *other);
	bool waitForIdle();
	
private:
	static FileType determineFileType(const std::string &fileName);
	static void closefd(int fd);
	void sendCommand(const std::string &cmd);
	bool startProcess(FileType fileType);
	void applyPlaybackSettings();
};

#endif // PLAYSOUND_H
