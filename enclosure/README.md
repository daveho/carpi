The enclosure designs were created with LibreCAD.

Here's how it looks completely assembled (click for large version)!

<a href="https://raw2.github.com/daveho/carpi/master/enclosure/pic-big.jpg"><img style="margin-left: 40px;" src="https://raw2.github.com/daveho/carpi/master/enclosure/pic-sm.jpg" /></a>

Files:

* plate2.dxf - the main drawing showing all of the features of the top and bottom plates of the enclosure, including the Raspberry Pi, the LCD, the buttons PCB, standoffs, etc.
* top2.dxf, bottom2.dxf - drawings with just the cuts needed for the top and bottom plates
* combined2.dxf - combined drawing of top and bottom plates; this can be submitted to your favorite laser cutting service (I personally think [Pololu](http://www.pololu.com/) is awesome!)
* ibex\_rpi\_model\_b\_v1\_00.dxf - CAD drawing of the Raspberry Pi,
  taken from http://www.raspberry-projects.com/pi/pi-hardware/model-b-cad
* ibex\_rpi\_model\_b\_v1\_00\_correct\_orientation.dxf -
  same as above, but shown as a top view instead of a bottom view
* lcd.dxf - drawing of just the LCD module, showing screw hole locations
* buttons2.dxf - drawing of outline and screw holes for buttons PCB (v0.2, with Cherry MX keyswitches)

The **old** subdirectory has a previous version of the enclosure where the buttons were 12mm x 12mm pushbuttons: not recommended, since I messed up the header orientation on the v0.1 buttons PCB, and it will be very difficult to connect it to the Raspberry Pi.
