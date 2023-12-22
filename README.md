# ON AIR Light Controller

## Overview
An Arduino project to set up an ESP8266 (using ESP-01S) to control a mildly hacked Paladone-branded ON AIR light.

## Project Contents
* **esp8266-on-air-light.ino** - an Arduino sketch to host a page that can serve as a control panel for the light. It has buttons for the onboard LED, a button simulating a"single tap"of the original power button, and a button simlating a double tap of the same power button. On the original hardware, a single tap switches between three modes: 1) soft power on to fill brightness, 2) soft pulsating, and 3) off. The "double-tap" facilitates going straight to the pulsing or turning off from the steadt on.
* **OnAirSign.svg** - A creative commons licensed SVG image used in the web page. (Sourced from https://freesvg.org/on-air-sign)
* **arduino_secrets.h.example** - A secrets file for handling the WiFi credentials to be used by the ESP32. Make a copy of this called "arduino_secrets.h" and update with the credentials. Be sure to add arduino_secrets.h to your .gitignore so that it is not accidenttally uploaded to github.
* **README.md** - This file

## Notes
* I created a basic glue-down holder for the ESP-01S used in this project. It's hosted here: https://www.thingiverse.com/thing:6371050
* I've also created an Instructable for the hardware portion of this project, here: https://www.instructables.com/Browser-controlled-ON-AIR-Light-Modification/

## To Do
- [ ] Switch to dynamic webserver and add REST (https://mischianti.org/how-to-create-a-rest-server-on-esp8266-and-esp32-startup-part-1/)
- [ ] Get circuit diagram working (https://www.tinkercad.com/things/h35XzLU3PY7-on-air-control) - Needs additional components
- [x] Write Instructable

## Sources, Credits, and Tips o'the Hat

* https://github.com/earlephilhower/arduino-littlefs-upload (ESP32 LittleFS file uploader for Arduino)
* https://www.instructables.com/How-to-use-the-ESP8266-01-pins/ (How to use TX & RX pins as extra GPIO on ESP-01S)
* https://www.infineon.com/dgdl/Infineon-Datasheet_ILD8150_ILD8150E-DataSheet-v01_01-EN.pdf (This is probably the LED driver on the original board. However, since it'the chip is not actually labeled, this is only a guess. Used for reference only.)
* https://www.andreagrandi.it/2020/12/16/how-to-safely-store-arduino-secrets/ (What it says on the tin. This is how to use a secrets file that will also work with github web editing.)
* https://freesvg.org/on-air-sign (The free SVG)
* https://elinux.org/RPi_GPIO_Interface_Circuits (Excellent reference on different control circuits for GPIO pins)
