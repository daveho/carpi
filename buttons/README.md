This directory has schematics, PCB layout, and firmware for the
board that has the pushbuttons on it.  An attiny2313 microcontroller
does the debouncing.  This board also has a header that connects to
the LCD.

**buttons.sch** and **buttons.brd** are the schematic and board layout,
in Eagle CAD.

**buttons.c** is the firmware that runs on the attiny2313.
**Makefile** will compile it (using avr-gcc) and program it to the
device (using avrdude).
