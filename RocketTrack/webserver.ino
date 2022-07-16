
#define STATIONMODE 1

#include "webserver.h"

#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <ESPmDNS.h>	

#include "/home/chris/Projects/Rocketry/TTGO Beam/RocketTrack/RocketTrack/network_creds.ino"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Replaces placeholder with LED state value
String processor(const String& var)
{
//	Serial.println("webserver process entry");
	
	char buffer[256];
	memset(buffer,0,sizeof(buffer));
	
	if(var=="LORA_MODE")
	{
		if(lora_mode==1)	sprintf(buffer,"High Rate");	else	sprintf(buffer,"Long Range");
	}
	else if(var=="BAT_STATUS")
	{
		if(axp.isChargeingEnable())
			sprintf(buffer,"\"Charging\"");
		else
			sprintf(buffer,"\"Discharging\"");
	}
	else if(var=="BAT_CURRENT")
	{
/*
		if(axp.isChargeingEnable())
			sprintf(buffer,"%.1f",axp.getBattChargeCurrent());
		else
			sprintf(buffer,"%.1f",axp.getBattDischargeCurrent());
*/
		sprintf(buffer,"%.1f",axp.getBattChargeCurrent()/1e3);
	}
	else if(var=="BAT_VOLTAGE")
	{
		sprintf(buffer,"%.3f",axp.getBattVoltage()/1000);
	}
	else if(var=="LATITUDE")
	{
		if(lat>0)	sprintf(buffer,"%2.6f N",lat/1e7);	else	sprintf(buffer,"%2.6f S",-lat/1e7);
	}
	else if(var=="LONGITUDE")
	{
		if(lon>0)	sprintf(buffer,"%3.6f E",lon/1e7);	else	sprintf(buffer,"%3.6f W",-lon/1e7);
	}
	else if(var=="ALTITUDE")
	{
		sprintf(buffer,"%.1f",height/1e3);
	}
	else if(var=="NUM_CHANNELS")
	{
		sprintf(buffer,"%d",numCh);
	}
	else if(var=="GPS_FIX")
	{
		sprintf(buffer,"%d",gpsFix);
	}
	else if(var=="HORIZONTAL_ACCURACY")
	{
		sprintf(buffer,"%.1f",hAcc);
	}
	else if(var=="SAT_NUMS")
	{
		int cnt=0;
		for(cnt=0;cnt<numCh;cnt++)
		{
			if(cnt!=(numCh-1))
				sprintf(buffer+strlen(buffer),"%d,",svid[cnt]);
			else
				sprintf(buffer+strlen(buffer),"%d",svid[cnt]);
		}
	}
	else if(var=="SAT_ELEVS")
	{
		int cnt=0;
		for(cnt=0;cnt<numCh;cnt++)
		{
			if(cnt!=(numCh-1))
				sprintf(buffer+strlen(buffer),"%d,",elev[cnt]);
			else
				sprintf(buffer+strlen(buffer),"%d",elev[cnt]);
		}
	}
	else if(var=="SAT_AZS")
	{
		int cnt=0;
		for(cnt=0;cnt<numCh;cnt++)
		{
			if(cnt!=(numCh-1))
				sprintf(buffer+strlen(buffer),"%d,",azim[cnt]);
			else
				sprintf(buffer+strlen(buffer),"%d",azim[cnt]);
		}
	}
	else if(var=="SAT_SNRS")
	{
		int cnt=0;
		for(cnt=0;cnt<numCh;cnt++)
		{
			if(cnt!=(numCh-1))
				sprintf(buffer+strlen(buffer),"%d,",cno[cnt]);
			else
				sprintf(buffer+strlen(buffer),"%d",cno[cnt]);
		}
	}
	
//	Serial.println("webserver process exit");
	
	if(strlen(buffer)>0)
		return(buffer);
	else
		return String();
}

int SetupWebServer(void)
{
	// Initialize SPIFFS
	if(!SPIFFS.begin(true))
	{
		Serial.println("An Error has occurred while mounting SPIFFS");
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
	
#if STATIONMODE
	// Connect to existing Wi-Fi network
	WiFi.begin(ssid, password);
	while(WiFi.status()!=WL_CONNECTED)
	{
		delay(1000);
		Serial.println("Connecting to WiFi...");
	}
	
	// Print ESP32 Local IP Address
	Serial.println(WiFi.localIP());
#else
	// Act as an Access Point

	Serial.print("Setting AP (Access Point)...");
	
	// Remove the password parameter, if you want the AP (Access Point) to be open
	WiFi.softAP(ssid,password);

	IPAddress IP=WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(IP);
#endif
	
	if(!MDNS.begin("esp32"))
	{
		Serial.println("Error starting mDNS");
		return(1);
	}
	
	// Route for root / web page
	server.on("/",HTTP_GET,[](AsyncWebServerRequest *request)					{														request->redirect("/status.html");		});	
//	server.on("/",HTTP_GET,[](AsyncWebServerRequest *request)					{	Serial.println("Returning /");						request->send(200,"text/plain","Hello World!");	});
	
	server.on("/status.html",HTTP_GET,[](AsyncWebServerRequest *request)		{	Serial.println("Returning /status.html");			request->send(SPIFFS,"/status.html");	});	
	server.on("/status.css",HTTP_GET,[](AsyncWebServerRequest *request)			{	Serial.println("Returning /status.css");			request->send(SPIFFS,"/status.css");	});	
//	server.on("/status.js",HTTP_GET,[](AsyncWebServerRequest *request)			{	Serial.println("Returning /status.js");				request->send(SPIFFS,"/status.js");		});	
	
	server.on("/status.js",HTTP_GET,[](AsyncWebServerRequest *request)
	{
		Serial.println("Returning modified /status.js");
//		request->send(SPIFFS,"/status.js");
		request->send(SPIFFS,"/status.js",String(),false,processor);
	});	
	
	server.on("/config.html",HTTP_GET,[](AsyncWebServerRequest *request)		{	Serial.println("Returning /config.html");			request->send(SPIFFS,"/config.html");	});	
	server.on("/config.css",HTTP_GET,[](AsyncWebServerRequest *request)			{	Serial.println("Returning /config.css");			request->send(SPIFFS,"/config.css");	});	
	server.on("/config.js",HTTP_GET,[](AsyncWebServerRequest *request)			{	Serial.println("Returning /config.js");				request->send(SPIFFS,"/config.js");		});	
	
	server.on("/engineering.html",HTTP_GET,[](AsyncWebServerRequest *request)	{	Serial.println("Returning /engineering.html");		request->send(SPIFFS,"/engineering.html");		});	
	server.on("/engineering.css",HTTP_GET,[](AsyncWebServerRequest *request)	{	Serial.println("Returning /engineering.css");		request->send(SPIFFS,"/engineering.css");		});	
	server.on("/engineering.js",HTTP_GET,[](AsyncWebServerRequest *request)		{	Serial.println("Returning /engineering.js");		request->send(SPIFFS,"/engineering.js");		});	
	
	server.on("/pton.html",HTTP_POST,[](AsyncWebServerRequest *request)
	{
		Serial.println("Turning permanent transmit ON");
		lora_constant_transmit=true;
		request->redirect("/engineering.html");
	});	
	
	server.on("/ptoff.html",HTTP_POST,[](AsyncWebServerRequest *request)
	{
		Serial.println("Turning permanent transmit off");
		lora_constant_transmit=false;
		request->redirect("/engineering.html");
	});	
	
	server.on("/longrange.html",HTTP_POST,[](AsyncWebServerRequest *request)
	{
		Serial.println("Setting to Long Range mode");
		SetLoRaMode(0);
		request->redirect("/engineering.html");
	});	
	
	server.on("/highrate.html",HTTP_POST,[](AsyncWebServerRequest *request)
	{
		Serial.println("Setting to High Rate mode");
		SetLoRaMode(1);
		request->redirect("/engineering.html");
	});	
	
	// Start server
	server.begin();
	
	return(0);
}

void PollWebServer(void)
{
	
}
