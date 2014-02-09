This directory has schematics, PCB layout, and firmware for the
board that has the pushbuttons on it.  An attiny2313 or attiny4313
microcontroller does the debouncing.  This board also has a header
that connects to the LCD.

**buttons2.sch** and **buttons2.brd** are the schematic and board layout,
in Eagle CAD.

Rendering (via [OSH Park](http://www.oshpark.com), which is where I had the boards fabricated):

<a href="https://raw2.github.com/daveho/carpi/master/buttons/pcbBottom.png"><img alt="bottom of PCB" src="https://raw2.github.com/daveho/carpi/master/buttons/pcbBottom-sm.png" /></a>

<a href="https://raw2.github.com/daveho/carpi/master/buttons/pcbBottom.png"><img alt="top of PCB" src="https://raw2.github.com/daveho/carpi/master/buttons/pcbBottom-sm.png" /></a>

Note that the PCB uses Cherry MX keyboard switches.  **cherrymx.lbr** is a
(very slightly) modified version of the following Eagle library:
https://github.com/c0z3n/cherrymx-eagle

[I sent a pull request with my changes, which were purely improvements
to the schematic symbol.]

**buttons.c** is the firmware that runs on the attiny2313.
**Makefile** will compile it (using avr-gcc) and program it to the
device (using avrdude).
