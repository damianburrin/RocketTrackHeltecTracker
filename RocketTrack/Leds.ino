
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

void ControlLED(axp_chgled_mode_t Mode)
{
	static axp_chgled_mode_t OldMode=AXP20X_LED_OFF;

	if(Mode!=OldMode)
	{
		axp.setChgLEDMode(Mode);
		
		OldMode=Mode;
	}
}

void PollLEDs(void)
{
	if(millis()>=NextLEDs)
	{
//		ControlLED(AXP20X_LED_BLINK_1HZ);
		NextLEDs=millis()+1000L;
	}
}

