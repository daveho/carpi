Hardware design and code for Raspberry Pi based car entertainment
device.  This is very much a work in progress!

Everything (software and hardware) is open source: see the file
LICENSES.txt for details.

The basic idea is being able to play videos and music
from the Raspberry Pi.  The user interface will consist
of a 2.2" LCD and 6 pushbuttons.  A sandwich-style
enclosure consisting of two pieces of laser-cut acrylic
and various bits of hardware is planned.  The "enclosure"
directory has (hopefully) complete CAD drawings.

Current status of the software is

* It's a console (ncurses) app.  This probably won't change,
  although the software makes a clean separation between
  controllers and views, and the former are completely
  independent of the latter, so a proper GUI interface
  could be supported.
* The LCD display is assumed to be 240x320, accessed as
  a Linux framebuffer device with fbcon.  (Specifically,
  an SPI-based LCD using the ILI9340 controller chip,
  supported by fbtft using the "adafruit22a" driver.
  Note that the LCD I'm using isn't specifically the
  Adafruit one, but appears to be completely compatible.)
  With a 6x12 font this gives us a 40x26 text display.
* Music playing works using mpg321 and ogg123 via the
  remote control interface (actually ogg123 playback doesn't
  work, need to investigate this).
* Pushbutton input will be supported using the RPi's
  GPIO (probably using the wiringPi library).
* Video playing will be supported via omxplayer, with
  composite output (because composite video monitors for
  cars are super cheap).

Current status of the hardware is

* Enclosure design is complete and I have submitted it for
  laser cutting.
* PCB design (for the buttons and LCD connector) is complete and
  has been submitted for fabrication.
