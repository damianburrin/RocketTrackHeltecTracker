#!/bin/bash

rm data/*~ 2>/dev/null

mkspiffs -c data -b 4096 -p 256 -s 0x170000 rockettrack.spiffs.bin

~/arduino-cli/arduino-cli compile \
	--fqbn esp32:esp32:t-beam \
	RocketTrack.ino 
