#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H

#include "abstract_event_visitor.h"

class Menu;

class MenuController : public AbstractEventVisitor
{
private:
	Menu *m_menu;
	
public:
	MenuController(Menu *menu);
	~MenuController();
	
	Menu *getMenu() { return m_menu; }

	void visitButtonEvent(ButtonEvent *evt);
};

#endif // MENUCONTROLLER_H
