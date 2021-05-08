
uint8_t batvolt=0x00;

void SetupPMIC(void)
{
	Serial.print("AXP192 Init");
	if(!axp.begin(Wire, AXP192_SLAVE_ADDRESS))	{	Serial.println(" PASS");	} 
	else                                        {	Serial.println(" FAIL");	}
	
	axp.setPowerOutPut(AXP192_LDO2,AXP202_ON);
	axp.setPowerOutPut(AXP192_LDO3,AXP202_ON);
	axp.setPowerOutPut(AXP192_DCDC2,AXP202_ON);
	axp.setPowerOutPut(AXP192_EXTEN,AXP202_ON);
	axp.setPowerOutPut(AXP192_DCDC1,AXP202_ON);
	axp.setDCDC1Voltage(3300);
	
	Serial.printf("\tDCDC1 2v5: %s\n",axp.isDCDC1Enable()?"ENABLE":"DISABLE");
	Serial.printf("\tDCDC2 Not Used: %s\n",axp.isDCDC2Enable()?"ENABLE":"DISABLE");
	Serial.printf("\tLDO2 LoRa: %s\n",axp.isLDO2Enable()?"ENABLE":"DISABLE");
	Serial.printf("\tLDO3 GPS: %s\n",axp.isLDO3Enable()?"ENABLE":"DISABLE");
	Serial.printf("\tDCDC3 3v3: %s\n",axp.isDCDC3Enable()?"ENABLE":"DISABLE");
	Serial.printf("\tExten: %s\n",axp.isExtenEnable()?"ENABLE":"DISABLE");

	if(axp.isChargeingEnable())	{	Serial.println("Charging is enabled");	}
	else						{	Serial.println("Charging is disabled");	}
	
	axp.adc1Enable(AXP202_BATT_CUR_ADC1,true);
}

void PollPMIC(void)
{
	static uint32_t updateat=0;
	
	if(millis()>updateat)
	{
		float batteryvoltage=axp.getBattVoltage();
		Serial.printf("Battery voltage = %.3f",batteryvoltage);
		
		float batterycurrent=axp.getBattChargeCurrent();
		
		// scale so 4250mV or fully charge gives a value of 212
		batvolt=(uint8_t)(batteryvoltage/20);
		
		if(axp.isChargeing())	{	Serial.printf(", charging at %.1f mA ...\n",batterycurrent);	}
		else					{	Serial.print("\n");												}
		
		updateat=millis()+1000L;
	}
}
