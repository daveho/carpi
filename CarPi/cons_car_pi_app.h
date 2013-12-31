#ifndef CONSCARPIAPP_H
#define CONSCARPIAPP_H

#include "car_pi_app.h"

class ConsCarPiApp : public CarPiApp
{
private:
	typedef CarPiApp Base;
	
public:
	ConsCarPiApp();
	~ConsCarPiApp();
	
	static void initialize();

protected:
	virtual void onStartMainLoop();
	virtual void onEndMainLoop();
	virtual EventHandler *createMenuView(Menu *menu);
};

#endif // CONSCARPIAPP_H
