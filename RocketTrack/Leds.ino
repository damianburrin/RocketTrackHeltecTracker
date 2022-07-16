
bool leds_enable=true;
uint32_t NextLEDs=0;

uint32_t LedPattern=0;
uint16_t LedRepeatCount=0;
uint8_t LedBitCount=0;

int SetupLEDs(void)
{
	Serial.println("Setting LED off");
	ControlLED(AXP20X_LED_OFF);

	LedPattern=0xffffffff;
	LedRepeatCount=2;
	LedBitCount=0;					

	return(0);
}

void PollLEDs(void)
{
	if(leds_enable)
	{
		if(millis()>=NextLEDs)
		{
			if(LedPattern&0x8000000)	ControlLED(AXP20X_LED_LOW_LEVEL);
			else						ControlLED(AXP20X_LED_OFF);
			
			if(LedRepeatCount>0)
			{
				LedPattern=(LedPattern<<1)|(LedPattern>>31);
			}
			else
			{
				LedPattern=(LedPattern<<1);
			}
			
			LedBitCount++;
			if(LedBitCount>=32)
			{
				if(LedRepeatCount>0)	LedRepeatCount--;
				LedBitCount=0;
			}
			
			// run the leds on a Hz cycle
			NextLEDs=millis()+200L;
		}
	}
}

int LEDCommandHandler(uint8_t *cmd,uint16_t cmdptr)
{
	// ignore a single key stroke
	if(cmdptr<=2)	return(0);

#if (DEBUG>0)
	Serial.println((char *)cmd);
#endif
	
	int retval=1;
	
	switch(cmd[1]|0x20)
	{
		case 't':	Serial.print("Triggering LED Test pattern\r\n");
					LedPattern=0b11101110111000111010111;
					LedRepeatCount=3;
					LedBitCount=0;
					break;
		
		case '0':	LedPattern=0x00000000;
					LedRepeatCount=0xffff;
					LedBitCount=0;
					break;
					
		case '1':	LedPattern=0xffffffff;
					LedRepeatCount=0xffff;
					LedBitCount=0;
					break;
					
		default:	// ignore
					retval=0;
					break;
	}
	
	return(retval);
}

