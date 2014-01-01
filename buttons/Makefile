PROG = buttons
MCU = attiny2313

CC = avr-gcc
CFLAGS = -Os -Wall -mmcu=$(MCU) -funsigned-bitfields -fpack-struct -fshort-enums -std=gnu99
OBJCOPY = avr-objcopy

all : $(PROG).hex

$(PROG).o : $(PROG).c
	$(CC) $(CFLAGS) -c $(PROG).c -o $(PROG).o

$(PROG).elf : $(PROG).o
	$(CC) -mmcu=$(MCU) $(PROG).o -o $@ -Wl,-Map=$(PROG).map,--cref -lm

$(PROG).hex : $(PROG).elf
	$(OBJCOPY) -O ihex -R .eeprom $(PROG).elf $(PROG).hex

clean :
	rm -f *.o *.elf *.hex *.map
