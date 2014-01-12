#! /bin/bash

# Unexport GPIO pins
PINS="2 3 4 17 27 22"
for p in $PINS; do echo $p > /sys/class/gpio/unexport; done
