
#include "SD.h"

bool sdcard_enable=true;

int SetupSDCard(void)
{
#if 1
	Serial.print("SD card disabled in this build ...\r\n");
	return(1);
#else
	while(1)
	{
		Serial.print("Initializing SD card...");
		
		pinMode(SDCARD_NSS,OUTPUT);
		
		if(!SD.begin(SDCARD_NSS))
		{
			Serial.println("initialization failed!");
			sdcard_enable=false;
	//		return(1);
		}
		
		Serial.println("initialization done.");	
		
		delay(2000);
	}
	
	return(0);
#endif
}

