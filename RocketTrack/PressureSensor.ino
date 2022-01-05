
bool psensor_enable=true;

uint32_t NextPsensor=0;

int SetupPressureSensor(void)
{
	
	return(0);
}

void PollPressureSensor(void)
{
	if(psensor_enable)
	{
		if(millis()>=NextPsensor)
		{
			// read the sensor
			
			
			// 25Hz measurement rate
			NextPsensor=millis()+40L;
		}
	}	
}

