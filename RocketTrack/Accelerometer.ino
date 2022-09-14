
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include <string.h>

int acc_enable=1;

char acc_type[32]="None";

int acc_rate=200;

Adafruit_MPU6050 mpu;

int SetupAccelerometer(void)
{
	Serial.println("SetupAccelerometer() entry");

//	if(strstr(acc_type,"None")!=NULL)
//	{
//		Serial.println("No accelerometer configured, disabling");
//		acc_enable=0;	
//	}
//	else if(strstr(acc_type,"MPU6050")!=NULL)
//	{
		if(!mpu.begin())
		{
			Serial.println("MPU6050 not detected, disabling");
			acc_enable=0;
			return(1);
		}
		
		mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
		mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
		
		Serial.println("MPU6050 configured");
//	}
//	else if(strstr(acc_type,"MPU9250")!=NULL)
//	{
//	
//	}
//	else
//	{
//		Serial.println("No accelerometer configured, disabling");
//		acc_enable=0;
//	}
	
	Serial.println("SetupAccelerometer() exit");
	
	return(0);
}

void PollAccelerometer(void)
{
	if(acc_enable)
	{
//		if(strstr(acc_type,"MPU6050")!=NULL)
		{
			sensors_event_t a,g,temp;
			mpu.getEvent(&a,&g,&temp);

			Serial.print("Acceleration X: ");	Serial.print(a.acceleration.x);	Serial.print(", Y: ");	Serial.print(a.acceleration.y);	Serial.print(", Z: ");	Serial.print(a.acceleration.z);	Serial.print(" m/s^2\t");
			Serial.print("Rotation X: ");		Serial.print(g.gyro.x);			Serial.print(", Y: ");	Serial.print(g.gyro.y);			Serial.print(", Z: ");	Serial.print(g.gyro.z);			Serial.print(" rad/s\t");
			Serial.print("Temperature: ");		Serial.print(temp.temperature);	Serial.print(" degC\t");
//			Serial.println("");
						
//			delay(50);
		}
//		else if(strstr(acc_type,"MPU9250")!=NULL)
		{
		
		}
//		else
//		{
//			Serial.println("No accelerometer configured, disabling");
//			acc_enable=0;
//		}
	}
}
