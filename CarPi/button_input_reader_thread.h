#ifndef BUTTON_INPUT_READER_THREAD_H
#define BUTTON_INPUT_READER_THREAD_H

#include "thread.h"

class GpioPin;

class ButtonInputReaderThread : public Thread {
private:
	GpioPin *m_pinList;

public:
	ButtonInputReaderThread();
	virtual ~ButtonInputReaderThread();

	// Call this before starting the thread
	bool initGpio();

	virtual void run();
};

#endif // BUTTON_INPUT_READER_THREAD_H
