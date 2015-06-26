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

//#define DEBUG_PLAYSOUND

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <cassert>
#include <algorithm>
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
#include "playback_settings.h"
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

PlaySoundCallback::PlaySoundCallback()
{
}

PlaySoundCallback::~PlaySoundCallback()
{
}

PlaySound::MonitorThread::MonitorThread(PlaySound *ps, PlaySound::FileType fileType)
	: m_ps(ps)
	, m_fileType(fileType)
	, m_currentStatus(PlaySoundCallback::STOPPED)
{
}

PlaySound::MonitorThread::~MonitorThread()
{
}

void PlaySound::MonitorThread::run()
{
	FILE *f = fdopen(m_ps->m_statusfd, "r");

	PlaySoundCallback *callback = m_ps->m_callback;

	std::string line;
	while (StringUtil::readLine(f, line)) {
		if (callback == 0) {
			continue; 
		}
		
		// See README.remote from mpg321 documentation

		if (StringUtil::startsWith(line, "@F ")) {
			// A file is definitely playing if we receive this message.
			updateStatus(PlaySoundCallback::PLAYING);
			
			int curFrame, remainingFrames;
			int curTime, remainingTime;
			std::vector<std::string> tokens = StringUtil::tokenize(line.substr(3, line.size()-3));
			if (tokens.size() == 4) {
				curFrame = atoi(tokens[0].c_str());
				remainingFrames = atoi(tokens[1].c_str());
				curTime = parseHundredths(tokens[2]);
				remainingTime = parseHundredths(tokens[3]);
				callback->onFrame(curFrame, remainingFrames, curTime, remainingTime);
				
				if (m_fileType == PlaySound::OGG && remainingTime < 100) {
					// XXX: ogg123 does not send a @P message to let us know that playing has ended!
					// So, as a complete hack, generate the status update synthetically
					// after an appropriate delay.  Blech.
					usleep(((unsigned long) remainingTime) * 10000UL);
					updateStatus(PlaySoundCallback::ENDED);
				}
			}
		} else if (StringUtil::startsWith(line, "@P ")) {
			int playStatus;
			if (sscanf(line.c_str() + 3, "%d", &playStatus) == 1) {
				updateStatus(static_cast<PlaySoundCallback::PlayStatus>(playStatus));
			}
		} else if (StringUtil::startsWith(line, "@I ID3:")) {
			// Parse ID3 information
			std::string title, artist, album;
			parseId3(line.c_str() + 7, title, artist, album);
			callback->onID3(title, artist, album);
		} else if (StringUtil::startsWith(line, "@I ")) {
			// Parse file name
			size_t pos = line.find_last_of('/');
			std::string fileName = (pos != std::string::npos) ? line.substr(pos+1, line.size()) : line.substr(3, line.size());
			callback->onFileName(fileName);
		}
	}
	
	fclose(f);
}

void PlaySound::MonitorThread::updateStatus(PlaySoundCallback::PlayStatus status)
{
	if (status == m_currentStatus) {
		return;
	}
	m_currentStatus = status;
	m_ps->m_callback->onPlayStatus(status);
}

void PlaySound::MonitorThread::parseId3(const std::string &line, std::string &title, std::string &artist, std::string &album)
{
	// Each field is exactly 30 characters
	// Examples:
	// @I ID3:Julia                         Asylum Party                  Picture One                   1989                              Coldwave
	// @I ID3:Ticket To Ride                Asylum Party                  The Grey Years Vol 2 (CD 1)   2006                              New Wave
	// line contains just the part after "@I ID3:"
	size_t len = line.size();
	title = StringUtil::trimSpaces(line.substr(0, 30));
	artist = (len > 30) ? StringUtil::trimSpaces(line.substr(30, 30)) : "";
	album = (len > 60) ? StringUtil::trimSpaces(line.substr(60, 30)) : "";
}

int PlaySound::MonitorThread::parseHundredths(const std::string &s)
{
	int val = 0;
	int sec, h;
	if (sscanf(s.c_str(), "%d.%d", &sec, &h) == 2) {
		// Note: we're assuming here that the part after the decimal point
		// is exactly 2 digits.  If it's not, then this computation won't
		// be accurate.  However, it doesn't matter since we're only
		// interested in second-level timing.
		if (h > 99) {
			h = 99;
		}
		val = sec*100 + h;
	} else if (sscanf(s.c_str(), "%d", &sec) == 1) {
		val = sec*100;
	}
	return val;
}

PlaySound::PlaySound()
	: m_state(IDLE)
	, m_selectedFile(0)
	, m_pid(-1)
	, m_cmdfd(-1)
	, m_statusfd(-1)
	, m_monitor(0)
	, m_callback(0)
	, m_playbackSettings(new PlaybackSettings())
{
}

PlaySound::~PlaySound()
{
}

void PlaySound::addFile(const std::string &fileName)
{
	m_fileList.push_back(fileName);
}

bool PlaySound::play(size_t i)
{
	if (i >= m_fileList.size()) {
		return false;
	}
	std::string fileName(m_fileList[i]);
	FileType fileType = determineFileType(fileName);

	if (m_state < ACTIVE) {
		startProcess(fileType);
		applyPlaybackSettings();
	} else if (m_state == PLAYING) {
		sendCommand("stop\n");
	}
	
	// Hack for ogg123 weirdness
	if (fileType == OGG) {
		//sendCommand("stop\n");
	}
	
	sendCommand("load " + fileName + "\n");
	m_state = PLAYING;
	m_selectedFile = i;
	return true;
}

bool PlaySound::previous()
{
	return (m_selectedFile > 0) ? play(m_selectedFile - 1) : false;
}

bool PlaySound::next()
{
	return (m_fileList.size() > 1 && m_selectedFile < m_fileList.size()-1) ? play(m_selectedFile+1) : false;
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

bool PlaySound::updatePlaybackSettings(const PlaybackSettings *other)
{
	if (*m_playbackSettings != *other) {
		*m_playbackSettings = *other;
		if (m_state > IDLE) {
			applyPlaybackSettings();
		}
	}
	return true;
}

bool PlaySound::waitForIdle()
{
	if (m_state != EXITING) {
		return false;
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
	delete m_monitor;
	m_monitor = 0;
	DEBUG("completed\n");
	
	// Resume idle state
	m_state = IDLE;
	m_pid = -1;
	m_cmdfd = -1;
	m_statusfd = -1;
	
	return true;
}


PlaySound::FileType PlaySound::determineFileType(const std::string &fileName)
{
	if (StringUtil::endsWith(fileName, ".mp3")) {
		return MP3;
	} else if (StringUtil::endsWith(fileName, ".ogg")) {
		return OGG;
	} else {
		// Unknown file extension
		return UNKNOWN;
	}
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

bool PlaySound::startProcess(FileType fileType)
{
	bool rc = false;

	assert(m_state == IDLE);
	
	std::string playerExe;
	switch (fileType) {
		case MP3: playerExe = MPG321_EXE_PATH; break;
		case OGG: playerExe = OGG123_EXE_PATH; break;
		default: assert(false);
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
		m_monitor = new MonitorThread(this, fileType);
		m_monitor->start();

		m_state = ACTIVE;

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

void PlaySound::applyPlaybackSettings()
{
	sendCommand("gain " + std::to_string(m_playbackSettings->getVolumePercent()) + "\n");
}
