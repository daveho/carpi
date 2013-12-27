#ifndef MUSICPLAYERCONTROLLER_H
#define MUSICPLAYERCONTROLLER_H

#include <vector>
#include <string>
#include "abstract_event_visitor.h"

class MusicPlayerController : public AbstractEventVisitor
{
private:
	typedef std::vector<std::string> FileList;
	FileList m_fileList;
	
public:
	MusicPlayerController();
	~MusicPlayerController();
	
	void addFile(const std::string &fileName) {
		m_fileList.push_back(fileName);
	}

	virtual void visitButtonEvent(ButtonEvent *evt);
	virtual void visitNotificationEvent(NotificationEvent *evt);
};

#endif // MUSICPLAYERCONTROLLER_H
