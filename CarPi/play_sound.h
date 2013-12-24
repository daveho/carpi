// CarPi - Raspberry Pi car entertainment system
// Copyright (c) 2013, David H. Hovemeyer <david.hovemeyer@gmail.com>

// This file is part of CarPi.
// 
// CarPi is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
// 
// Foobar is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with CarPi.  If not, see <http://www.gnu.org/licenses/>.

#ifndef PLAYSOUND_H
#define PLAYSOUND_H

#include <string>
#include <sys/types.h>

#include "thread.h"

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

	State m_state;
	pid_t m_pid;
	int m_cmdfd;    // pipe to send commands to subprocess
	int m_statusfd; // pipe to receive status updates from subprocess
	//pthread_t m_monitor;
	MonitorThread *m_monitor;
	
public:
	PlaySound();
	~PlaySound();

	State getState() { return m_state; }
	
	bool play(const std::string &fileName);
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
