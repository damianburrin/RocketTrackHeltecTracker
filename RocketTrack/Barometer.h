
#pragma once

extern int baro_enable;
extern char baro_type[];
extern int baro_rate;

int SetupBarometer(void);
void PollBarometer(void);

