# arduino-custom-7segment-clock
Embedded Arduino clock with RTC, shift-register-driven 7-segment display, speech synthesis (Talkie), and sensor integration.

# Arduino Talking Clock with Custom 7-Segment Display

<p align="center">
  <img src="lcd_demo.jpg" width="400">
</p>

## Overview

This project is a custom-designed Arduino clock built as part of a university art/engineering project. The goal was to create a visually unique digital clock using 7-segment displays with a custom aesthetic.

Instead of using standard segments, the design is based on a stylized segment layout originally created by Posy. The design was adapted and made fully 3D printable.

In addition to displaying time, the clock includes speech output, environmental sensing, and interactive features.

## Demo

YouTube video:
https://www.youtube.com/watch?v=VrYwxv41JsA&t=730s

## Features

* Custom 3D-printed 7-segment display design
* RTC-based timekeeping
* Speech output using Talkie library
* Button-based time adjustment
* PWM brightness control
* Automatic brightness adjustment using LDR
* Temperature and humidity sensing (DHT)
* Random simple math question feature
* Modular enclosure design (printable without supports)

## 3D Design

The segment design is inspired by:

* https://www.youtube.com/watch?v=RTB5XhjbgZA&t=18s
* http://www.michieldb.nl/other/segments/

The enclosure and parts were adapted to be fully 3D printable:

* No supports required
* Modular top and side panels
* Digits can be printed separately and reused in other projects

## Printing Notes

* Diffusers: white filament, ~0.2 mm thick bottom layer
* Body: bronze PLA
* Segments are hollow; LEDs are inserted from the back
* Tolerances are tight — test prints are recommended

Filament change tip:

* Use a pause command (e.g. G4), but avoid pausing directly over the part
* Prefer pausing over a skirt or sacrificial area to avoid surface defects

## Electronics Notes

* LEDs are driven via shift registers and PNP transistors
* Logic is inverted (LOW = ON, HIGH = OFF)
* Power consumption is relatively high due to brightness requirements

Power considerations:

* Long USB cables may cause voltage drop and reset loops
* A boost converter (5V → 6V) is used for the speaker amplifier
* The system includes a switch to toggle between 5V and 6V

Alternative:

* Lower-value resistors can be used if skipping the boost converter

## Libraries Used

Install the following libraries before compiling:

* Talkie
  https://github.com/ArminJo/Talkie

* uRTCLib
  https://github.com/Naguissa/uRTCLib

* ClickButton
  https://github.com/marcobrianza/ClickButton

* DHT Sensor Library (Adafruit recommended)
  https://github.com/adafruit/DHT-sensor-library

* Shifty (Shift Register Control)
  https://github.com/johnnyb/arduino-shifty

## Notes

This project was developed as both a technical and artistic exploration. It combines embedded programming, hardware design, and 3D printing into a single system.

The code and hardware evolved over time, so some parts may not be perfectly clean or optimized. Feel free to experiment or adapt the design.

## Author

Mert Kaya
