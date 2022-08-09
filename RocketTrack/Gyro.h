
#pragma once

extern int gyro_enable;
extern char gyro_type[];
extern int gyro_rate;

int SetupGyro(void);
void PollGyro(void);
