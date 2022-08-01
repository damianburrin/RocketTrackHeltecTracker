
#pragma once

extern int wifi_enable;

extern int wifi_mode;

int SetupWiFi(void);

extern char ssid[];
extern char password[];

extern char apssid[];
extern char appassword[];

extern IPAddress LocalIP;
extern IPAddress Gateway;
extern IPAddress Subnet;
extern IPAddress DNS1;
extern IPAddress DNS2;
