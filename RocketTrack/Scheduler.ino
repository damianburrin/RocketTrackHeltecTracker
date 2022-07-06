
#define USER_BUTTON			38

uint32_t next_transmit=0;

int SetupScheduler(void)
{
	pinMode(USER_BUTTON,INPUT);
	if(digitalRead(USER_BUTTON)==0)
		lora_constant_transmit=true;
	
	return(0);
}

bool last_user_button=true;
int button_timer=0;

void PollScheduler(void)
{
	bool user_button=digitalRead(USER_BUTTON);
	
	if(!user_button&&last_user_button)
	{
		Serial.println("Button pressed");
		button_timer=millis();
	}
	
	if(user_button&&!last_user_button)
	{
		Serial.println("Button released");
		
		if(millis()>(button_timer+1000))
		{
			lora_constant_transmit=!lora_constant_transmit;
			Serial.print("Setting constant transmit to ");
			Serial.println(lora_constant_transmit);
		}
		else
		{
			lora_mode=!lora_mode;
			Serial.print("Toggling lora_mode to ");
			Serial.println(lora_mode);
		}
	}
	
	last_user_button=user_button;
	
	if(millis()>=next_transmit)
	{
		if(		(LoRaTransmitSemaphore==0)
			&&	(lora_constant_transmit)	)
		{
			PackPacket();
			EncryptPacket(TxPacket);
			TxPacketLength=16;
			LoRaTransmitSemaphore=1;
			
			if(lora_mode==0)	next_transmit=millis()+10000;
			else				next_transmit=millis()+1000;
			
			Serial.printf("millis() = %d\r\n",millis());
		}
	}
}

