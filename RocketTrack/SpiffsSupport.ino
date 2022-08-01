
#include "SPIFFS.h"

int spiffs_enable=1;

int SetupSPIFFS(void)
{
	// Initialize SPIFFS
	if(!SPIFFS.begin(true))
	{
		Serial.println("An error has occurred while mounting SPIFFS");
		return(1);
	}
	
#if 0
	File root=SPIFFS.open("/");
	File file=root.openNextFile();
	
	while(file)
	{
		Serial.print("FILE: ");
		Serial.println(file.name());
		
		file=root.openNextFile();
	}
#endif
#if 0
	File status=SPIFFS.open("/status.html");
	if(!status)
	{
		Serial.println("Error opening /status.html");
	}
	else
	{
//    while(status.available())

      uint8_t buf[64];
      status.read(buf,64);
			Serial.print((char *)buf);
    		
		close(status);
	}
#endif
		
	return(0);
}

