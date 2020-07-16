## RocketTrack

This is a fork of Dave Akerman's FlexTrack customised for use on a high power rocket using a TTGO T-Beam v1.0 or equivalent board fitted with an SD Card socket.

## Features 

- Operates in two modes; High Speed and Long Range.  

		Long Range mode is for when the flight has completed and the rocket is on the ground and will give the best range at a limited update rate.  
		High Speed mode is for before and during the flight when it records high rate GPS data to the SD card before it detects landing and switches to Long Range mode.

- Uses binary mode uBlox protocol from the GPS receiver to simplify onboard processing

- Is configured from a simple web interface served over WiFi

- Stores configuration in a file stored on the SD Card

- Optionally uses strong encryption

- Stores high rate gyro, accelerometer and magnetometer data to the SD card logs

- Can use a barometric altimeter, BMP180 or BME280 initially, and store and transmit this data




























#### Legacy below here



## FlexTrack 

Flexible Arduino-based tracker software for RTTY, LoRa and APRS.

This fork is configured for Habduino V4.X boards. 

This code provides simultaneous (where legal and supported by the hardware installed) transmission of radio telemetry via RTTY and APRS on Habduino Arduino addon boards for High Altitude Ballooning.

## Libraries

To build FlexTrack for use with a HABDuino or other tracker using I2C communications, you need to install this library into your Arduino IDE:

[https://github.com/rambo/I2C](https://github.com/rambo/I2C) 

In addition, for the TTGO T-Beam the AXP202X Library is required:

[https://github.com/lewisxhe/AXP202X_Library](https://github.com/lewisxhe/AXP202X_Library)

Libraries can be added via the Ardunio IDE menu:  Sketch -> Include Library -> Add .zip Library

## Disclaimer

The FlexTrack code is provided as is with no guarantees of performance or operation. 

If you decide to use this code under a balloon it’s your responsibility to ensure you comply with the local legislation and laws regarding meteorological balloon launching and radio transmission in the air. 
The Radiometrix NTX2B 434Mhz is NOT license exempt in the United States of America and does need a radio amateur license.

Use of APRS requires a radio amateur license in all countries and a number of countries don’t permit the airborne use of APRS under any circumstances. 

It is YOUR responsibility to ensure Habduino hardware and code is used safely and legally please review the safety section on the website. 

## Further Reading on High Altitude Ballooning

Please read this http://www.daveakerman.com/?p=1732

## License

The hardware design & code for Habduino is released under a Creative Commons License 3.0 Attribution-ShareAlike License : http://creativecommons.org/licenses/by-sa/3.0/
