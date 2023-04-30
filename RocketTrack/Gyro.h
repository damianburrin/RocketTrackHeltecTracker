
#pragma once

extern int gyro_enable;
extern char gyro_type[];
extern int gyro_rate;

int SetupGyro(void);
void PollGyro(void);

enum
{
	GYRO_NONE=0,
	GYRO_MPU6050,
	GYRO_MPU9250
};
