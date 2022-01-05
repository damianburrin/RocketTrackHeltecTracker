
bool leds_enable=true;
uint32_t NextLEDs=0;

// LED modes:
// ON at startup
// 4Hz till GPS lock
// 1Hz with GPS lock
// OFF above 100m (for power save)

int SetupLEDs(void)
{
//	ControlLED(AXP20X_LED_LOW_LEVEL);
//	ControlLED(AXP20X_LED_OFF);

	return(0);
}

void PollLEDs(void)
{
	if(leds_enable)
	{
		if(millis()>=NextLEDs)
		{
			// do something, dunno what just yet
			
			// run the leds on a 10Hz cycle
			NextLEDs=millis()+100L;
		}
	}
}

