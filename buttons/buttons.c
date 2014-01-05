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
// The buttons should connect to ground when pressed

#define F_CPU 1000000 // use internal oscillator, set to 1 MHz
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef USE_LOW_POWER_MODE
#  include <avr/sleep.h>
#endif
#include <util/delay.h>

// Input level must be stable for at least this many
// time units, where a time unit is 1/4096 sec.
#define DEBOUNCE_TIME 16

// Inputs
// Button 1 - PD6
// Button 2 - PB0
// Button 3 - PB1
// Button 4 - PB2
// Button 5 - PB3
// Button 6 - PB4

// Outputs (debounced button values, 0=not pressed, 1=pressed)
// Button 1 - PD0
// Button 2 - PD1
// Button 3 - PA1
// Button 4 - PA0
// Button 5 - PD2
// Button 6 - PD3

// Bit masks for output pins on ports A and D
#define PORTA_OUTPUT_MASK (_BV(PA0)|_BV(PA1))
#define PORTD_OUTPUT_MASK (_BV(PD0)|_BV(PD1)|_BV(PD2)|_BV(PD3))

// Bit masks for input pins on ports B and D
#define PORTB_INPUT_MASK (_BV(PB0)|_BV(PB1)|_BV(PB2)|_BV(PB3)|_BV(PB4))
#define PORTD_INPUT_MASK _BV(PD6)

uint8_t g_cur = 0x3F;      // current input values
uint8_t g_last = 0x3F;     // last input values
uint8_t g_count[6];        // count of how many polls buttons have had their current values
uint8_t g_out = 0x3F;      // current output values

#ifndef NDEBUG
uint16_t g_debug_count = 0;
#endif

void read_inputs(void);
void update_counts(void);
void write_outputs(void);

// Timer interrupt, called once per 1ms
ISR(TIMER0_OVF_vect)
{
	read_inputs();
	update_counts();
	write_outputs();
	g_last = g_cur;

#ifndef NDEBUG
	// Blink LED on PD4 for debugging
	g_debug_count++;
	if (g_debug_count & 0x400) {
		PORTD ^= _BV(PD4);
	}
#endif
}

int main(void)
{
	// Ensure all pins on port B are input
	DDRB = 0;

	// Enable PA0-1, PD0-3 as outputs
	DDRA = PORTA_OUTPUT_MASK;
	DDRD = PORTD_OUTPUT_MASK;
#ifndef NDEBUG
	DDRD |= _BV(PD4);
#endif

	// Enable internal pull-up resistors on PB0-4, PD6
	PORTB = PORTB_INPUT_MASK;
	PORTD = PORTD_INPUT_MASK;

	// Set up timer interrupt to fire 4096 times per second
	TCCR0B |= (1<<CS00);   // no prescaling
	TIMSK |= (1 << TOIE0); // interrupt on overflow of counter 0

	// Enable interrupts
	sei();

	// At this point, the interrupt handler should
	// do all of the work.
	for (;;) {
#ifdef USE_LOW_POWER_MODE
		// The main loop attempts to spend as
		// much time as possible in low-power mode.
		cli();
		sleep_enable();
		sei();
		sleep_cpu();
#endif
	}
}

void read_inputs(void)
{
	// Process buttons in reverse order: button 1 is the
	// least significant bit
	uint8_t inb = PINB;
	uint8_t ind = PIND;
	g_cur = 0;

	// Read input for button 6
	g_cur |= ((inb & _BV(PB4)) != 0); // button 6
	g_cur <<= 1;

	// Read input for buttons 5-1
	g_cur |= ((inb & _BV(PB3)) != 0); // button 5
	g_cur <<= 1;
	g_cur |= ((inb & _BV(PB2)) != 0); // button 4
	g_cur <<= 1;
	g_cur |= ((inb & _BV(PB1)) != 0); // button 3
	g_cur <<= 1;
	g_cur |= ((inb & _BV(PB0)) != 0); // button 2
	g_cur <<= 1;
	g_cur |= ((ind & _BV(PD6)) != 0); // button 1
}

void update_counts(void)
{
	// Update counts, allow outputs to change if count is
	// sufficiently high
	uint8_t nextout = g_out;
	for (uint8_t i = 0, mask = 1; i < 6; i++, mask <<= 1) {
		if ((g_cur & mask) == (g_last & mask)) {
			// Button input value is same as last poll: increment count
			if (g_count[i] < 255) { g_count[i]++; }
		} else {
			// Button input value is different from last poll:
			// reset count to 0
			g_count[i] = 0;
		}

		// Allow output to change if count is sufficiently high
		if (g_count[i] > DEBOUNCE_TIME && (g_cur & mask) != (g_out & mask)) {
			nextout ^= mask;
		}
	}

	g_out = nextout;
}

void write_outputs(void)
{
	// Get current port A/D values, but leaving 0s for the bits
	// corresponding to the button outputs we want to generate
	uint8_t outa = PORTA & ~PORTA_OUTPUT_MASK;
	uint8_t outd = PORTD & ~PORTD_OUTPUT_MASK;

	// Set button output bits in port A/D as appropriate.
	// Note that the button inputs are 0=pressed, but we generate
	// the debounced outputs as 1=pressed.
	uint8_t genout = g_out;
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
