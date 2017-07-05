// CarPi - Raspberry Pi car entertainment system
// Copyright (c) 2013-2017 David H. Hovemeyer <david.hovemeyer@gmail.com>

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

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "thread.h"
#include "playback_settings.h"
#include "string_util.h"
#include "play_video.h"

// Default path to omxplayer executable.
#ifndef OMXPLAYER_EXE_PATH
#  define OMXPLAYER_EXE_PATH "/usr/bin/omxplayer"
#endif

namespace {
	class MonitorThread : public Thread {
	private:
		PlayVideo *m_pv;
		int m_fd;
		
	public:
		MonitorThread(PlayVideo *pv, int fd);
		virtual ~MonitorThread();
		
		virtual void run();
	};
	
	MonitorThread::MonitorThread(PlayVideo *pv, int fd)
		: m_pv(pv)
		, m_fd(fd)
	{
		
	}
	
	MonitorThread::~MonitorThread()
	{
		
	}
	
	void MonitorThread::run()
	{
		FILE *fd = fdopen(m_fd, "r");
		while (true) {
			std::string line;
			if (!StringUtil::readLine(fd, line)) {
				break;
			}
			// TODO: parse status
		}
		// TODO: send end of stream notification
		fclose(fd);
	}
}

PlayVideoCallback::PlayVideoCallback()
{
}

PlayVideoCallback::~PlayVideoCallback()
{
}

PlayVideo::PlayVideo(const std::string &fileName)
	: m_state(IDLE)
	, m_fileName(fileName)
	, m_pid(-1)
	, m_cmdfd(-1)
	, m_statusfd(-1)
	, m_errfd(-1)
	, m_stdoutMonitor(0)
	, m_stderrMonitor(0)
	, m_playbackSettings(new PlaybackSettings())
	, m_callback(0)
{
}

PlayVideo::~PlayVideo()
{
	if (m_state != IDLE) {
		sendCommand("q");
		waitForIdle();
		m_stdoutMonitor->join();
		delete m_stdoutMonitor;
		m_stderrMonitor->join();
		delete m_stderrMonitor;
	}
}

bool PlayVideo::play()
{
	if (m_state < PLAYING) {
		startProcess();
		return true;
	} else if (m_state == PAUSED) {
		sendCommand("p");
		return true;
	} else {
		return false;
	}
}

bool PlayVideo::seek(SeekDir dir, SeekSize size)
{
	if (m_state < PLAYING) {
		return false;
	}
#define X(dir,size) ((dir*2)+size)
	switch (X(dir,size)) {
	case X(BACKWARD, SMALL):
		sendCommand("\033[D"); break;
	case X(BACKWARD, LARGE):
		sendCommand("\033[B"); break;
	case X(FORWARD, SMALL):
		sendCommand("\033[C"); break;
	case X(FORWARD, LARGE):
		sendCommand("\033[A"); break;
	}
#undef X
	return true;
}

bool PlayVideo::pause()
{
	if (m_state != PLAYING) {
		return false;
	}
	sendCommand("p");
	m_state = PAUSED;
	return true;
}

bool PlayVideo::stop()
{
	if (m_state == IDLE) {
		return false;
	}
	sendCommand("q");
	m_state = EXITING;
	return true;
}

bool PlayVideo::waitForIdle()
{
	if (m_state != EXITING) {
		return false;
	}
	
	// close pipes to subprocess
	close(m_cmdfd);
	close(m_statusfd);
	close(m_errfd);
	
	// wait for subprocess to exit
	int status;
	waitpid(m_pid, &status, 0);
	
	// wait for monitor threads to finish
	m_stdoutMonitor->join();
	delete m_stdoutMonitor;
	m_stdoutMonitor = 0;
	m_stderrMonitor->join();
	delete m_stderrMonitor;
	m_stderrMonitor = 0;
	
	return true;
}

bool PlayVideo::updatePlaybackSettings(const PlaybackSettings *other)
{
	if (*m_playbackSettings != *other) {
		*m_playbackSettings = *other;
	}
	return true;
}

bool PlayVideo::startProcess()
{
	extern char **environ;
	bool rc = false;
	int cmdpipe[2] = {-1, -1};     // pipe to send commands to omxplayer
	int statuspipe[2] = {-1, -1};  // pipe to receive stdout from omxplayer
	int errpipe[2] = {-1, -1};     // pipe to receive stderro from omxplayer
	
	// Construct command
	const char *args[6];
	args[0] = OMXPLAYER_EXE_PATH;
	args[1] = "-b";  // show a blank background
	args[2] = "-o";
	args[3] = (m_playbackSettings->getOutputType() == PlaybackSettings::COMPOSITE)
		? "local"
		: "hdmi";
	args[4] = m_fileName.c_str();
	args[5] = 0;
	
	// Create pipes
	if (pipe(cmdpipe) < 0 || pipe(statuspipe) < 0 || pipe(errpipe) < 0) {
		goto done;
	}
	
	// Fork the command
	pid_t pid;
	pid = fork();
	if (pid < 0) {
		// error starting process
		goto done;
	} else if (pid > 0) {
		// in parent
		
		// dup one side of each pipe
		m_cmdfd = dup(cmdpipe[1]);
		m_statusfd = dup(statuspipe[0]);
		m_errfd = dup(errpipe[0]);
		
		// start monitor threads
		m_stdoutMonitor = new MonitorThread(this, m_statusfd);
		m_stdoutMonitor->start();
		m_stderrMonitor = new MonitorThread(this, m_errfd);
		m_stderrMonitor->start();
		
		// success!
		m_pid = pid;
		m_state = PLAYING;
		rc = true;
	} else {
		// in child
		
		// set up one side of each pipe as stdin/stdout/stderr
		dup2(cmdpipe[0], 0);
		dup2(statuspipe[1], 1);
		dup2(errpipe[1], 2);
		
		// close pipes
		close(cmdpipe[0]);
		close(cmdpipe[1]);
		close(statuspipe[0]);
		close(statuspipe[1]);
		close(errpipe[0]);
		close(errpipe[1]);
		
		// execute omxplayer!
		execve(OMXPLAYER_EXE_PATH, (char **)args, environ);
		
		// whoops
		exit(1);
	}
	
done:
	closefd(cmdpipe[0]);
	closefd(cmdpipe[1]);
	closefd(statuspipe[0]);
	closefd(statuspipe[1]);
	closefd(errpipe[0]);
	closefd(errpipe[1]);
	
	return rc;
}

void PlayVideo::sendCommand(const std::string &cmd)
{
	write(m_cmdfd, cmd.c_str(), cmd.size());
}

void PlayVideo::closefd(int fd)
{
	if (fd >= 0) {
		close(fd);
	}
}
