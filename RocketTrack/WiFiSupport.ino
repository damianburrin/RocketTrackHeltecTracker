
#define STATIONMODE 1

#include "WiFiSupport.h"

#include "WiFi.h"

char ssid[32]="ROO";
char password[64]="sausageeggchipsandbeans";

char apssid[32]="RocketTrack";
char appassword[64]="eggsbenedict";

IPAddress LocalIP(192,168,0,2);
IPAddress Gateway(192,168,0,1);
IPAddress Subnet(255,255,255,0);
IPAddress DNS1(8,8,8,8);
IPAddress DNS2(8,8,4,4);
	
int wifi_enable=1;

int wifi_mode=0;		// infrastructure mode

int SetupWiFi(void)
{
	if(wifi_mode==0)
	{
		// try connecting to infrastructure first
		
		if(strlen(ssid)==0)
		{
			Serial.println("Error, ssid is zero-length, aborting ...");	
			return(1);
		}
		
		WiFi.config(LocalIP,Gateway,Subnet,DNS1,DNS2);
		
		if(strlen(password)!=0)
			WiFi.begin(ssid,password);
		else
		{
			Serial.println("CAUTION: using open wifi settings!");
			WiFi.begin(ssid);
		}

		int failcnt=0;
		while(WiFi.status()!=WL_CONNECTED)
		{
			delay(500);
			Serial.print("Connecting to WiFi AP ");	Serial.print(ssid);	Serial.println(" ...");
			failcnt++;
			if(failcnt>=20)
			{
				Serial.println("WiFi connection failed, aborting ...");
				wifi_mode=1;
				break;
			}
		}
		
		// Print ESP32 Local IP Address
		Serial.println(WiFi.localIP());
	}

	if(wifi_mode==1)
	{
		// Act as an Access Point

		Serial.print("Setting AP (Access Point)...");
		
		WiFi.softAPConfig(LocalIP,Gateway,Subnet);
		
		// Remove the password parameter, if you want the AP (Access Point) to be open
		if(strlen(appassword)!=0)
			WiFi.softAP(apssid,appassword);
		else
		{
			Serial.println("CAUTION: using open wifi settings!");
			WiFi.softAP(apssid);
		}

		IPAddress IP=WiFi.softAPIP();
		
		Serial.print("Starting AP mode with SSID = ");	Serial.println(apssid);
		Serial.print("on IP address = ");				Serial.println(IP);

#if 0
		if(!MDNS.begin("tracker"))
		{
			Serial.println("Error starting mDNS");
			return(1);
		}
#endif
	}
	
	return(0);
}