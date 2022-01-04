## RocketTrack

This is a heavily modified fork of Dave Akerman's FlexTrack customised for
use on a high power rocket using a TTGO T-Beam v1.0 or equivalent board
fitted with an SD Card socket.

## Features 

- Operates in two modes; High Speed and Long Range.  

	Long Range mode is for when the flight has completed and the 
	rocket is on the ground and will give the best range at a limited 
	update rate.  
	
	High Speed mode is for before and during the flight when it records
	high rate GPS data to the SD card before it detects landing and 
	switches to Long Range mode.

- Uses binary mode uBlox protocol from the GPS receiver to simplify onboard processing

- Runs the GPS receiver at 5Hz fix rate with 1Hz satellite visibility data in the logged stream

- May log raw pseudo-range and carrier phase data from the GPS receiver for later processing using RTKLib to refine the track through the sky

- Will be configured from a simple web interface served over WiFi

- Stores configuration in a file stored on the SD Card

- Optionally uses strong encryption

- Stores high rate gyro, accelerometer and magnetometer data, if available, to the SD card logs

- Can use a barometric altimeter, BMP180 or BME280 initially, and store and transmit this data












## To Do

Secure wipe of plaintext

Enables for encryption, logging etc.

Some sort of web configuration page

Consider including RocketFlightReceiver into this so there's only one codebase

Do some sort of receiver display that looks like a compass etc. for actually
tracking rather than just presenting the received fix and the current local
fix.  This will involve great circle code and some webby type stuff.

Prediction of the landing point during descent

Flight events detection to switch from high rate to long range mode

Flight events as part of the data packet

Logging format in transmitter mode and receiver mode

Potentially control of the transmitter from the receiver for reconfiguration

Use of pulsed carrier, pulsed FM tone burst to allow tracking using non-LoRa
receivers

Pairing mode to allow two devices to find each other and configure
themselves to work together

Further work on the test harness e.g. load crypto keys, frequencies, set
parameters for long range and high rate mode

Storage and retrieval of configuration from a file, potentially on either
the SD card if used or from a SPIFFS on the chip



## Libraries

To build FlexTrack for use with a HABDuino or other tracker using I2C communications, you need to install this library into your Arduino IDE:

[https://github.com/rambo/I2C](https://github.com/rambo/I2C) 

In addition, for the TTGO T-Beam the AXP202X Library is required:

[https://github.com/lewisxhe/AXP202X_Library](https://github.com/lewisxhe/AXP202X_Library)

Libraries can be added via the Ardunio IDE menu:  Sketch -> Include Library -> Add .zip Library

## Disclaimer

The RocketTrack code is provided as is with no guarantees of performance or operation. 

## License

The hardware design & code for Habduino is released under a Creative Commons License 3.0 Attribution-ShareAlike License : http://creativecommons.org/licenses/by-sa/3.0/
