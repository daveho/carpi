// CarPi - Raspberry Pi car entertainment system
// Copyright (c) 2013,2014 David H. Hovemeyer <david.hovemeyer@gmail.com>

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

#define GPIO_TEST

#ifdef GPIO_TEST
#  include <sys/select.h>
#  include <cstdio>
#  include <cstdlib>
#  include "gpio_pin.h"
#else
#  include "cons_car_pi_app.h"
#endif

int main(void)
{
#ifdef GPIO_TEST
	GpioPin pinList[6];

	/*
	 * Button 1: GPIO2 (P1-3)
	 * Button 2: GPIO3 (P1-5)
	 * Button 3: GPIO4 (P1-7)
	 * Button 4: GPIO17 (P1-11)
	 * Button 5: GPIO27 (P1-13)
	 * Button 6: GPIO22 (P1-15)
	 */
	pinList[0].initForReading(2);
	pinList[1].initForReading(3);
	pinList[2].initForReading(4);
	pinList[3].initForReading(17);
	pinList[4].initForReading(27);
	pinList[5].initForReading(22);

	// Check that initializing the pins succeeded
	for (int i = 0; i < 6; i++) {
		if (!pinList[i].isActive()) {
			printf("Pin %d not active?\n", pinList[i].getPinNumber());
			exit(1);
		}
	}
	printf("Pins initialized correctly?\n");
	
#else
	ConsCarPiApp::initialize();
	CarPiApp::instance()->mainLoop();
#endif
	return 0;
}
