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

// Use attiny2313 to debounce 6 pushbuttons
// The buttons should connect to ground

#define F_CPU 1000000 // use internal oscillator, set to 1 MHz
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Input level must be stable for at least this many
// milliseconds to propagate to output
#define DEBOUNCE_TIME_MS 5

// Inputs
// Button 1 - PB4
// Button 2 - PB3
// Button 3 - PB2
// Button 4 - PB1
// Button 5 - PB0
// Button 6 - PD6

// Outputs (debounced button values, 0=not pressed, 1=pressed)
// Button 1 - PD0
// Button 2 - PD1
// Button 3 - PA1
// Button 4 - PA0
// Button 5 - PD2
// Button 6 - PD3

uint8_t cur = 0x3F;      // current input values
uint8_t last = 0x3F;     // last input values
uint8_t count[6];        // count of how many polls buttons have had their current values
uint8_t out = 0x3F;      // current output values

void read_inputs(void);
void update_counts(void);
void write_outputs(void);

int main(void)
{
	// Ensure all pins on port B are input
	DDRB = 0;

	// Enable PA0-1, PD0-3 as outputs
	DDRA = (_BV(PA0) | _BV(PA1));
	DDRD = (_BV(PD0) | _BV(PD1) | _BV(PD2) | _BV(PD3));
#ifndef NDEBUG
	DDRD |= _BV(PD4);
#endif

	// Enable internal pull-up resistors on PB0-4, PD6
	PORTB = (_BV(PB0) | _BV(PB1) | _BV(PB2) | _BV(PB3) | _BV(PB4));
	PORTD = _BV(PD6);

#ifndef NDEBUG
	uint8_t count = 0;
#endif

	for (;;) {
		read_inputs();
		update_counts();
		//out = cur; // no debouncing
		write_outputs();
		last = cur;
		_delay_ms(1);

#ifndef NDEBUG
		// Blink LED on PD4 for debugging
		count++;
		PORTD ^= (((count & 128) != 0) ? _BV(PD4) : 0);
#endif
	}
}

void read_inputs(void)
{
	// Process buttons in reverse order: button 1 is the
	// least significant bit

	cur = 0;

	// Read input for button 6
	uint8_t ind = PIND;
	cur |= ((ind & _BV(PD6)) != 0);
	cur <<= 1;

	// Read input for buttons 5-1
	uint8_t inb = PINB;
	cur |= ((inb & _BV(PB0)) != 0);
	cur <<= 1;
	cur |= ((inb & _BV(PB1)) != 0);
	cur <<= 1;
	cur |= ((inb & _BV(PB2)) != 0);
	cur <<= 1;
	cur |= ((inb & _BV(PB3)) != 0);
	cur <<= 1;
	cur |= ((inb & _BV(PB4)) != 0);
}

void update_counts(void)
{
	// Update counts, allow outputs to change if count is
	// sufficiently high
	uint8_t nextout = out;
	for (uint8_t i = 0, mask = 1; i < 6; i++, mask <<= 1) {
		if ((cur & mask) == (last & mask)) {
			// Button input value is same as last poll: increment count
			if (count[i] < 255) { count[i]++; }
		} else {
			// Button input value is different from last poll:
			// reset count to 0
			count[i] = 0;
		}

		// Allow output to change if count is sufficiently high
		if (count[i] > DEBOUNCE_TIME_MS && (cur & mask) != (out & mask)) {
			nextout ^= mask;
		}
	}

	out = nextout;
}

void write_outputs(void)
{
	// Get current port A/D values, but leaving 0s for the bits
	// corresponding to the button outputs we want to generate
	uint8_t outa = PORTA & ~(_BV(PA0) | _BV(PA1));
	uint8_t outd = PORTD & ~(_BV(PD0) | _BV(PD1) | _BV(PD2) | _BV(PD3));

	// Set button output bits in port A/D as appropriate.
	// Note that the button inputs are 0=pressed, but we generate
	// the debounced outputs as 1=pressed.
	uint8_t genout = out;
	if ((genout & 1) == 0) { outd |= _BV(PD0); }
	genout >>= 1;
	if ((genout & 1) == 0) { outd |= _BV(PD1); }
	genout >>= 1;
	if ((genout & 1) == 0) { outa |= _BV(PA1); }
	genout >>= 1;
	if ((genout & 1) == 0) { outa |= _BV(PA0); }
	genout >>= 1;
	if ((genout & 1) == 0) { outd |= _BV(PD2); }
	genout >>= 1;
	if ((genout & 1) == 0) { outd |= _BV(PD3); }

	// Update outputs!
	PORTA = outa;
	PORTD = outd;
}
