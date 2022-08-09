
#pragma once

extern int acc_enable;
extern char acc_type[];
extern int acc_rate;

int SetupAccelerometer(void);
void PollAccelerometer(void);

