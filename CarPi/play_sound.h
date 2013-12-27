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

#include <vector>
#include <string>
#include <sys/types.h>

#include "thread.h"

//
// Callback interface to receive asynchronous status updates
// from the PlaySound subprocess.
//
class PlaySoundCallback {
public:
	enum PlayStatus {
		STOPPED = 0,
		PAUSED = 1,
		RESUMED = 2,
		ENDED = 3,
	};

	PlaySoundCallback();
	virtual ~PlaySoundCallback();
	
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
	enum State {
		IDLE,     // subprocess not running
		PLAYING,  // subprocess is playing sound file
		PAUSED,   // subprocess is paused
		EXITING,  // subprocess is exiting
	};
	
private:
	//
	// Monitor thread class
	//
	class MonitorThread : public Thread {
	private:
		PlaySound *m_ps;
		
	public:
		MonitorThread(PlaySound *ps);
		virtual ~MonitorThread();
		virtual void run();
	};

	typedef std::vector<std::string> FileList;
	FileList m_fileList;
	State m_state;
	pid_t m_pid;
	int m_cmdfd;    // pipe to send commands to subprocess
	int m_statusfd; // pipe to receive status updates from subprocess
	MonitorThread *m_monitor;
	PlaySoundCallback *m_callback;
	
public:
	PlaySound();
	~PlaySound();
	
	void addFile(const std::string &fileName);
	
	void setCallback(PlaySoundCallback *callback) { m_callback = callback; }

	State getState() const { return m_state; }
	
	bool play(/*const std::string &fileName*/);
	bool pause();
	bool resume();
	bool stop();
	void waitForIdle();
	
private:
	static void closefd(int fd);
	//static void *monitor(void *arg);
	void sendCommand(const std::string &cmd);
	bool startProcess(const std::string &fileName);
};

#endif // PLAYSOUND_H
