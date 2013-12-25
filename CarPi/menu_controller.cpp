#include "menu.h"
#include "event_queue.h"
#include "menu_controller.h"

MenuController::MenuController(Menu *menu)
	: m_menu(menu)
{
}

MenuController::~MenuController()
{
}

void MenuController::visitButtonEvent(ButtonEvent *evt)
{
	// There is nothing to do if the menu has no items
	if (getMenu()->getNumItems() == 0) {
		return;
	}
	
	if (evt->getType() == ButtonEvent::RELEASE) {
		size_t selected = getMenu()->getSelected();
		
		switch (evt->getCode()) {
			case ButtonEvent::UP:
				setResult(EventHandler::HANDLED);
				if (selected > 0) {
					selected--;
				}
				break;
			
			case ButtonEvent::DOWN:
				setResult(EventHandler::HANDLED);
				if (selected < getMenu()->getNumItems() - 1) {
					selected++;
				}
				break;
				
				// TODO: choosing an item
			
			default:
				break;
		}
		
		if (selected != getMenu()->getSelected()) {
			// Selection changed
			getMenu()->setSelected(selected);
			EventQueue::instance()->enqueue(new NotificationEvent(NotificationEvent::SELECTION_CHANGED));
		}
	}
}
