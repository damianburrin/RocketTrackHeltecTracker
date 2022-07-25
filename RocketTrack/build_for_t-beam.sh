#!/bin/bash

arduino-cli lib uninstall SD

rm webdata/*~ 2>/dev/null

mkspiffs -c webdata -b 4096 -p 256 -s 0x170000 rockettrack.spiffs.bin

arduino-cli compile \
	--fqbn esp32:esp32:t-beam \
	--verbose \
	RocketTrack.ino 
