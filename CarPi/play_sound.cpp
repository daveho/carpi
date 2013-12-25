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

//#define DEBUG_PLAYSOUND

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <cassert>
#ifdef DEBUG_PLAYSOUND
#  include <iostream>
#endif
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "string_util.h"
#include "thread.h"
#include "play_sound.h"

// default mpg321 and ogg123 executables
#ifndef MPG321_EXE_PATH
#  define MPG321_EXE_PATH "/usr/bin/mpg321"
#endif
#ifndef OGG123_EXE_PATH
#  define OGG123_EXE_PATH "/usr/bin/ogg123"
#endif

#ifdef DEBUG_PLAYSOUND
#  define DEBUG(args) std::cout << args << std::flush
#else
#  define DEBUG(args)
#endif

PlaySound::MonitorThread::MonitorThread(PlaySound *ps)
	: m_ps(ps)
{
}

PlaySound::MonitorThread::~MonitorThread()
{
}

void PlaySound::MonitorThread::run()
{
	FILE *f = fdopen(m_ps->m_statusfd, "r");

	// For now, just read the status information without parsing it.
	int c;
	while ((c = fgetc(f)) != EOF) {
		// do nothing
	}
	fclose(f);
}

PlaySound::PlaySound()
	: m_state(IDLE)
	, m_pid(-1)
	, m_cmdfd(-1)
	, m_statusfd(-1)
	, m_monitor(0)
{
}

PlaySound::~PlaySound()
{
}

bool PlaySound::play(const std::string &fileName)
{
	return startProcess(fileName);
}

bool PlaySound::pause()
{
	if (m_state != PLAYING) {
		return false;
	}
	sendCommand("pause\n");
	m_state = PAUSED;
	return true;
}

bool PlaySound::resume()
{
	if (m_state != PAUSED) {
		return false;
	}
	sendCommand("pause\n");
	m_state = PLAYING;
	return true;
}

bool PlaySound::stop()
{
	if (!(m_state == PLAYING || m_state == PAUSED)) {
		return false;
	}
	sendCommand("quit\n");
	m_state = EXITING;
	return true;
}

void PlaySound::waitForIdle()
{
	if (m_state != EXITING) {
		return;
	}
	
	// Close pipes to subprocess
	close(m_cmdfd);
	close(m_statusfd);
	
	// Wait for subprocess and monitor thread to exit
	int status;
	DEBUG("waiting for subprocess to exit...");
	waitpid(m_pid, &status, 0);
	DEBUG("exited\n");
	DEBUG("waiting for monitor thread to complete...");
	m_monitor->join();
	DEBUG("completed\n");
	
	// Resume idle state
	m_state = IDLE;
	m_pid = -1;
	m_cmdfd = -1;
	m_statusfd = -1;
}

void PlaySound::closefd(int fd)
{
	if (fd >= 0) {
		close(fd);
	}
}

void PlaySound::sendCommand(const std::string &cmd)
{
	if (write(m_cmdfd, cmd.c_str(), cmd.size()) < 0) {
		fprintf(stderr, "write to pipe failed: %s\n", strerror(errno));
	}
}

bool PlaySound::startProcess(const std::string &fileName)
{
	bool rc = false;
	std::string playerExe;

	assert(m_state == IDLE);

	// Determine which player to use
	if (StringUtil::endsWith(fileName, ".mp3")) {
		playerExe = MPG321_EXE_PATH;
	} else if (StringUtil::endsWith(fileName, ".ogg")) {
		playerExe = OGG123_EXE_PATH;
	} else {
		// Unknown file extension
		return false;
	}
	
	int cmdpipe_fd[2] = {-1, -1};
	int statuspipe_fd[2] = {-1, -1};
	extern char **environ;
	pid_t pid;

	const char *args[4];
	args[0] = playerExe.c_str();
	args[1] = "-R";   // Remote control interface!
	args[2] = "abcd"; // a dummy argument is required
	args[3] = 0;

	// create pipes that the parent can use to talk to the subprocess
	if (pipe(cmdpipe_fd) < 0 || pipe(statuspipe_fd) < 0) {
		goto done;
	}

	pid = fork(); // create a new process
	if (pid < 0) {
		// error
		goto done;
	} else if (pid > 0) {
		// in the parent process
		m_pid = pid;

		// Dup the pipe file descriptors that will send commands to
		// the subprocess and read status info from the subprocess.
		m_cmdfd = dup(cmdpipe_fd[1]);
		m_statusfd = dup(statuspipe_fd[0]);

		// start the monitor thread to read status info from subprocess
		m_monitor = new MonitorThread(this);
		m_monitor->start();

		// tell the child to start playing the song
		std::string playCommand("load " + fileName);
		sendCommand(playCommand);

		m_state = PLAYING;

		// success!
		rc = true;
	} else {
		// in the child process

		// connect mpg123's stdin to the read side of the command pipe
		dup2(cmdpipe_fd[0], 0);
		close(cmdpipe_fd[0]);
		close(cmdpipe_fd[1]);

		// connect mpg123's stdout to the write side of the status pipe
		dup2(statuspipe_fd[1], 1);
		dup2(statuspipe_fd[1], 2);
		close(statuspipe_fd[0]);
		close(statuspipe_fd[1]);

		// Exec!
		execve(playerExe.c_str(), (char **)args, environ);// if successful, does not return
		exit(1); // whoops
	}

done:
	closefd(cmdpipe_fd[0]);
	closefd(cmdpipe_fd[1]);
	closefd(statuspipe_fd[0]);
	closefd(statuspipe_fd[1]);

	return rc;
}
