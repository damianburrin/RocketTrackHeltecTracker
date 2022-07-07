
#include "webserver.h"

#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

// Replace with your network credentials
const char *ssid="SKYG8K8Y";			//Replace with your own SSID
const char *password="SKrz8jk6iiLq";	//Replace with your own password

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Replaces placeholder with LED state value
String processor(const String& var)
{
	Serial.println(var);
	
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
	
#if 1
	File root=SPIFFS.open("/");
	File file=root.openNextFile();
	
	while(file)
	{
		Serial.print("FILE: ");
		Serial.println(file.name());
		
		file=root.openNextFile();
	}
#endif
#if 1
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
	
	// Connect to Wi-Fi
	WiFi.begin(ssid, password);
	while(WiFi.status()!=WL_CONNECTED)
	{
		delay(1000);
		Serial.println("Connecting to WiFi..");
	}
	
	// Print ESP32 Local IP Address
	Serial.println(WiFi.localIP());
	
	// Route for root / web page
//	server.on("/",HTTP_GET,[](AsyncWebServerRequest *request)				{	Serial.println("Returning /");				request->redirect,"/status.html");		});	
	server.on("/",HTTP_GET,[](AsyncWebServerRequest *request)				{	Serial.println("Returning /");				request->send(200,"text/plain","Hello World!");	});
	
	server.on("/status.html",HTTP_GET,[](AsyncWebServerRequest *request)
	{
		Serial.println("Returning /status.html");
		
		File status=SPIFFS.open("/status.html");
		if(!status)
			Serial.println("Error opening /status.html");
		else
		{
			uint8_t buf[64];
			status.read(buf,64);
			Serial.print((char *)buf);
				
			close(status);
		}
		
		request->send(SPIFFS,"/status.html");	
	});	




	server.on("/status.css",HTTP_GET,[](AsyncWebServerRequest *request)		{	Serial.println("Returning /status.css");	request->send(SPIFFS,"/status.css");	});	
	server.on("/status.js",HTTP_GET,[](AsyncWebServerRequest *request)		{	Serial.println("Returning /status.js");		request->send(SPIFFS,"/status.js");		});	
	server.on("/config.html",HTTP_GET,[](AsyncWebServerRequest *request)	{	Serial.println("Returning /config.html");	request->send(SPIFFS,"/config.html");	});	
	server.on("/config.css",HTTP_GET,[](AsyncWebServerRequest *request)		{	Serial.println("Returning /config.css");	request->send(SPIFFS,"/config.css");	});	
	server.on("/config.js",HTTP_GET,[](AsyncWebServerRequest *request)		{	Serial.println("Returning /config.js");		request->send(SPIFFS,"/config.js");		});	
	
	// Start server
	server.begin();
	
	return(0);
}

void PollWebServer(void)
{
	
}
