
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

int gyro_enable=1;

char gyro_type[32]="None";
int gyro_type_no=0;

int gyro_rate=200;

extern Adafruit_MPU6050 mpu;

int SetupGyro(void)
{
	if(strstr(gyro_type,"None")!=NULL)
	{
		Serial.println("No gyro configured, disabling");
		gyro_enable=0;	
	}
	else if(strstr(gyro_type,"MPU6050")!=NULL)
	{
		if(!mpu.begin())
		{
			Serial.println("MPU6050 not detected, disabling");
			gyro_enable=0;
			return(1);
		}

		mpu.setGyroRange(MPU6050_RANGE_500_DEG);
		mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
		
		Serial.print("MPU6050 gyro configured\r\n");
		gyro_type_no=GYRO_MPU6050;
		gyro_enable=1;
	}
	else if(strstr(gyro_type,"MPU9250")!=NULL)
	{
	
	
	
	
	
	
	
	
		Serial.print("MPU6050 gyro configured\r\n");
		gyro_type_no=GYRO_MPU6050;
		gyro_enable=1;
	}
	else
	{
		Serial.println("No gyro configured, disabling");
		gyro_enable=0;
	}
	
	return(0);
}

void PollGyro(void)
{
	if(gyro_enable)
	{
		sensors_event_t a,g,temp;
		mpu.getEvent(&a,&g,&temp);
		Serial.print("Rotation X: ");		Serial.print(g.gyro.x);			Serial.print(", Y: ");	Serial.print(g.gyro.y);			Serial.print(", Z: ");	Serial.print(g.gyro.z);			Serial.print(" rad/s\t");
	}
}

