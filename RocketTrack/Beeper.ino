
bool beeper_enable=true;
uint32_t NextBeepers=0;

uint32_t BeeperPattern=0;
uint16_t BeeperRepeatCount=0;
uint8_t BeeperBitCount=0;

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
//			if(BeeperPattern&0x8000000)	ControlLED(AXP20X_LED_LOW_LEVEL);
//			else						ControlLED(AXP20X_LED_OFF);
			
			if(BeeperRepeatCount>0)
			{
				BeeperPattern=(BeeperPattern<<1)|(BeeperPattern>>31);
			}
			else
			{
				BeeperPattern=(BeeperPattern<<1);
			}
			
			BeeperBitCount++;
			if(BeeperBitCount>=32)
			{
				if(BeeperRepeatCount>0)	BeeperRepeatCount--;
				BeeperBitCount=0;
			}
						
			// run the beeper on a 5Hz cycle
			NextBeepers=millis()+200L;
		}
	}
}

int BeeperCommandHandler(uint8_t *cmd,uint16_t cmdptr)
{
	// ignore a single key stroke
	if(cmdptr<=2)	return(0);

#if (DEBUG>0)
	Serial.println((char *)cmd);
#endif
	
	int retval=1;
	
	switch(cmd[1]|0x20)
	{
		case 't':	Serial.print("Triggering Beeper Test pattern\r\n");
					BeeperPattern=0b11101110111000111010111;
					BeeperRepeatCount=3;
					BeeperBitCount=0;
					retval=0;
					break;
					
	
		default:	// ignore
					break;
	}
	
	return(retval);
}

