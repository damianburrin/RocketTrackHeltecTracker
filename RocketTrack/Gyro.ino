
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

int gyro_enable=1;

char gyro_type[32]="Generic";

int gyro_rate=200;

extern Adafruit_MPU6050 mpu;

int SetupGyro(void)
{
//	if(strstr(gyro_type,"None")!=NULL)
//	{
//		Serial.println("No gyro configured, disabling");
//		gyro_enable=0;	
//	}
//	else if(strstr(gyro_type,"MPU6050")!=NULL)
//	{
#if 0
		if(!mpu.begin())
		{
			Serial.println("MPU6050 not detected, disabling");
			gyro_enable=0;
			return(1);
		}
#endif
			
		mpu.setGyroRange(MPU6050_RANGE_500_DEG);
		mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
//	}
//	else if(strstr(gyro_type,"MPU9250")!=NULL)
//	{
//	
//	}
//	else
//	{
//		Serial.println("No gyro configured, disabling");
//		gyro_enable=0;
//	}
	
	return(0);
}

void PollGyro(void)
{

}

