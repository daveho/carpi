Hardware design and code for Raspberry Pi based car entertainment
device.  This is a work in progress, although the hardware and software
are more or less working at this point.  Some work remains to
get it into an easily installable and deployable form.

Everything (software and hardware) is open source: see the file
LICENSES.txt for details.

The basic idea is being able to play videos and music
from the Raspberry Pi.  The user interface consists
of a 2.2" LCD and 6 keyswitches.  A sandwich-style
enclosure consisting of two pieces of laser-cut acrylic
and various bits of hardware has been designed and assembled.
The "enclosure" directory has complete CAD drawings.

Here is a photo of the assembled enclosure (with the Raspberry Pi,
the LCD module, and the keyswitches):

<a href="https://raw2.github.com/daveho/carpi/master/enclosure/pic-big.jpg"><img src="https://raw2.github.com/daveho/carpi/master/enclosure/pic-sm.jpg" /></a>

Current status of the software is

* It's a console (ncurses) app.  This probably won't change,
  although the software makes a clean separation between
  controllers and views, and the former are completely
  independent of the latter, so a proper GUI interface
  could be supported.
* The LCD display is assumed to be 240x320, accessed as
  a Linux framebuffer device with fbcons.  (Specifically,
  an SPI-based LCD using the ILI9340 controller chip,
  supported by fbtft using the "adafruit22a" driver.
  Note that the LCD I'm using isn't specifically the
  Adafruit one, but appears to be completely compatible.)
  With a 8x16 font this gives us a 30x20 text display.
* Music playing works using mpg321 and ogg123 via the
  remote control interface (actually ogg123 playback doesn't
  work, need to investigate this).
* Pushbutton input is supported using the RPi's GPIO.
* Video playing is supported by running omxplayer as a
  subprocess, with video output going to the composite output
  (because composite video monitors for cars are super cheap).  

Things are pretty close to completely working: the main remaining
task is the video setup in the car.
