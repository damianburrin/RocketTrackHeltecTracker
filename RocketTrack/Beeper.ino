
bool beeper_enable=true;
uint32_t NextBeepers=0;

int SetupBeeper(void)
{
	if(beeper_enable)
	{
		// setup the pin we're gonna use to drive the beeper
		
		
	}
	
	return(0);
}

void PollBeeper(void)
{
	if(beeper_enable)
	{
		if(millis()>=NextBeepers)
		{
			// do something, dunno what just yet
			
			// run the leds on a 10Hz cycle
			NextBeepers=millis()+100L;
		}
	}
}

