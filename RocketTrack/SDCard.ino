
/*
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define SDCARD_NSS			4		// GPIO4 for select line to SD Card socket

bool sdcard_enable=true;

int SetupSDCard(void)
{
	SPI.begin(SCK,MISO,MOSI);
	
	if(!SD.begin(SDCARD_NSS))
	{
		Serial.println("Card Mount Failed");
		return(1);
	}
	
	uint8_t cardType=SD.cardType();
	if(cardType==CARD_NONE)
	{
		Serial.println("No SD card attached");
		return(1);
	}
	
	Serial.print("SD Card Type: ");
	if(cardType==CARD_MMC)
	{
		Serial.println("MMC");
	}
	else if(cardType==CARD_SD)
	{
		Serial.println("SDSC");
	}
	else if(cardType==CARD_SDHC)
	{
		Serial.println("SDHC");
	}
	else
	{
		Serial.println("UNKNOWN");
	}
	
	uint64_t cardSize = SD.cardSize() / (1024 * 1024);
	Serial.printf("SD Card Size: %lluMB\n", cardSize);
	
	return(0);
}
*/
