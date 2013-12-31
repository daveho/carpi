#include "console.h"
#include "cons_menu_view.h"
#include "cons_input_reader_thread.h"
#include "cons_car_pi_app.h"

ConsCarPiApp::ConsCarPiApp()
{
}

ConsCarPiApp::~ConsCarPiApp()
{
}

void ConsCarPiApp::initialize()
{
	setInstance(new ConsCarPiApp());
}

void ConsCarPiApp::onStartMainLoop()
{
	// Initialize console
	Console::initialize();
	Console::instance()->init();
	
	// Start the console input reader
	ConsInputReaderThread *inputReader = new ConsInputReaderThread();
	inputReader->start();
}

void ConsCarPiApp::onEndMainLoop()
{	
	// Clean up
	Console::instance()->cleanup();
}

EventHandler *ConsCarPiApp::createMenuView(Menu *menu)
{
	return new ConsMenuView(menu, 1, Console::instance()->getNumRows() - 2);
}
