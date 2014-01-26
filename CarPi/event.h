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

#ifndef EVENT_H
#define EVENT_H

#include <string>

class EventVisitor;

//
// Event base class.
//
class Event {
public:
	Event();
	virtual ~Event();
	
	virtual void accept(EventVisitor *visitor) = 0;
};

//
// Button event class.
// There are exactly 6 buttons.
//
class ButtonEvent : public Event {
public:
	enum Type {
		PRESS,
		RELEASE,
	};
	
	enum Code {
		// The arrows are used for menu navigation.
		// The left arrow typically goes back to the
		// previous menu.
		UP,
		DOWN,
		LEFT,
		RIGHT,

		// This button is play/pause for the player screens.
		// It is the button at the top left.
		A,

		// I think in the future this will be the "settings"
		// button, to bring up a menu to control volume, etc.
		// It is the button at the top left.
		B,      // TODO: think of a better name
	};
	
private:
	Type m_type;
	Code m_code;
	
public:
	ButtonEvent(Type type, Code code);
	virtual ~ButtonEvent();
	
	virtual void accept(EventVisitor *visitor);
	Type getType() const { return m_type; }
	Code getCode() const { return m_code; }
};

//
// Catch-all for various kinds of notifications.
//
class NotificationEvent : public Event {
public:
	enum Type {
		PAINT,             // render the view
		SELECTION_CHANGED, // menu selection changed
		MENU_CHANGED,      // menu changed (items added/removed)
		MENU_ITEM_SELECTED,// a menu item was selected
		PLAYER_STOPPED,    // player is stopped
		PLAYER_PAUSED,     // player is paused
		PLAYER_PLAYING,    // player is playing (or resumed)
		PLAYER_ENDED,      // player has ended
	};
	
private:
	Type m_type;
	const void *m_object;
	
public:
	NotificationEvent(Type type);
	NotificationEvent(Type type, const void *object);
	virtual ~NotificationEvent();
	
	Type getType() const { return m_type; }
	const void *getObject() const { return m_object; }
	
	virtual void accept(EventVisitor *visitor);
};

//
// Event with status information about a media file (sound or video).
//
class MediaFileInfoEvent : public Event {
private:
	std::string m_fileName;
	std::string m_title;
	std::string m_artist;
	std::string m_album;
	
public:
	MediaFileInfoEvent(const std::string &fileName);
	MediaFileInfoEvent(const std::string &title, const std::string &artist, const std::string &album);
	virtual ~MediaFileInfoEvent();
	
	bool hasMetadata() const { return m_fileName.empty(); }
	
	const std::string &getFileName() const { return m_fileName; }
	
	const std::string &getTitle() const { return m_title; }
	const std::string &getArtist() const { return m_artist; }
	const std::string &getAlbum() const { return m_album; }
	
	virtual void accept(EventVisitor *visitor);
};

//
// Event with status information about media (sound or video).
// Note that time is measured in hundredths of a second.
//
class MediaStatusEvent : public Event {
private:
	int m_curFrame;
	int m_remainingFrames;
	int m_curTime;
	int m_remainingTime;
	
public:
	MediaStatusEvent(int curFrame, int remainingFrames, int curTime, int remainingTime);
	virtual ~MediaStatusEvent();
	
	int getCurFrame() const { return m_curFrame; }
	int getRemainingFrames() const { return m_remainingFrames; }
	int getCurTime() const { return m_curTime; }
	int getRemainingTime() const { return m_remainingTime; }

	virtual void accept(EventVisitor *visitor);
};

class EventVisitor {
public:
	EventVisitor();
	virtual ~EventVisitor();
	virtual void visitButtonEvent(ButtonEvent *evt) = 0;
	virtual void visitNotificationEvent(NotificationEvent *evt) = 0;
	virtual void visitMediaFileInfoEvent(MediaFileInfoEvent *evt) = 0;
	virtual void visitMediaStatusEvent(MediaStatusEvent *evt) = 0;
};

#endif // EVENT_H
