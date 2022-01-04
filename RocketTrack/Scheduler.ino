
uint32_t next_transmit=0;

int SetupScheduler(void)
{
	
	
	return(0);
}

void PollScheduler(void)
{
	if(millis()>=next_transmit)
	{
		if(		(LoRaTransmit==0)
			&&	(lora_constant_transmit)	)
		{
			PackPacket();
			EncryptPacket(TXPacket);
			
			LoRaTransmit=1;
			
			if(lora_mode==0)	next_transmit=millis()+10000;
			else				next_transmit=millis()+333;
			
			Serial.printf("millis() = %d\r\n",millis());
		}
	}
}

