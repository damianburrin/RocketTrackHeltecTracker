
#define DEBUG 3

//#include <SPI.h>
#include <string.h>

#include "LoRa.h"
#include "Packetisation.h"

typedef enum {lmIdle, lmListening, lmSending} tLoRaMode;

bool LoRaTransmit=0;

uint32_t TXStartTimeMillis;

// LORA settings
#define LORA_FREQ			434650000
#define LORA_OFFSET			0         // Frequency to add in kHz to make Tx frequency accurate

// HARDWARE DEFINITION
#define LORA_NSS			18		// Comment out to disable LoRa code
#define LORA_RESET			14		// Comment out if not connected
#define LORA_DIO0			26

double lora_frequency=LORA_FREQ;
double lora_offset=LORA_OFFSET;
int lora_mode=0;

bool lora_constant_transmit=false;

uint8_t TxPacket[MAX_TX_PACKET_LENGTH];
uint8_t TxPacketLength;

uint16_t TxPacketCounter=0;

uint32_t LastLoRaTX=0;

int SetupLoRa(void)
{
	Serial.println("LoRa Sender");

    LoRa.setPins(LORA_NSS,LORA_RESET,LORA_DIO0);

//	if(!LoRa.begin(lora_frequency))
	if(!LoRa.begin(434.650E6))
	{
		Serial.println("Starting LoRa failed!");
		return(1);
	}
	else
	{
		Serial.println("Started LoRa ok ...");
		return(0);
	}

//	LoRa.enableInvertIQ();
//	LoRa.onTxDone(onTxDone);
}

void onTxDone()
{
	Serial.println("Tx Done!");
}

int LORACommandHandler(uint8_t *cmd,uint16_t cmdptr)
{
	// ignore a single key stroke
	if(cmdptr<=2)	return(0);

#if (DEBUG>0)
	Serial.println((char *)cmd);
#endif
	
	int retval=1;
	
	switch(cmd[1]|0x20)
	{
//		case 'x':	Serial.println(LoRa.getSignalBandwidth());
//					break;
					
		case 'd':	Serial.println("Dumping LoRa registers");
					LoRa.dumpRegisters(Serial);
					break;
		
		case 't':	Serial.println("Transmitting LoRa packet");
					strcpy((char *)TxPacket,"Hello, world!");
					TxPacketLength=strlen((char *)TxPacket);
//					memcpy(TxPacket,"$$Hello world!\r\n",16);
//					EncryptPacket(TxPacket);
//					TxPacketLength=16;
					LoRaTransmit=1;

#if 0					
					now=millis();
										
#if 0
					Serial.println("Setting LoRa parameters");
					LoRa.setTxPower(5);
					LoRa.setSpreadingFactor(7);
					LoRa.setSignalBandwidth(20.8E3);
					LoRa.setCodingRate4(8);
#endif
					
					LoRa.beginPacket(true);
					LoRa.write(TxPacket,TxPacketLength);
					LoRa.endPacket();

					Serial.print("\tblocking lora tx done in ");
					Serial.print(millis()-now);
					Serial.println(" ms");
#endif
					
					break;
		
		case 'g':	Serial.println("Transmitting GPS LoRa packet");
					PackPacket();
					EncryptPacket(TxPacket);
					TxPacketLength=16;
					LoRaTransmit=1;
					
					LoRa.beginPacket();
					LoRa.write(TxPacket,TxPacketLength);
					LoRa.endPacket(true);

					break;
		
		case 'l':	Serial.println("Long range mode");
					lora_mode=0;
					break;
		
		case 'h':	Serial.println("High rate mode");
					lora_mode=1;
					break;
		
		case 'm':	if(lora_mode==0)	Serial.print("Long range mode\r\n");
					else				Serial.print("High rate mode\r\n");
					break;
					
		case 'c':	lora_constant_transmit=!lora_constant_transmit;
					Serial.printf("Setting constant transmit mode to %d\r\n",lora_constant_transmit);
					break;
		
		case '+':	lora_offset+=1.0;
					Serial.printf("LoRa offset = %.1f\n",lora_offset);
					break;
		
		case '-':	lora_offset-=1.0;
					Serial.printf("LoRa offset = %.1f\n",lora_offset);
					break;
		
		case '?':	Serial.print("LoRa Test Harness\r\n================\r\n\n");
					Serial.print("t\t-\tTransmit a test packet\r\n");
					Serial.print("g\t-\tTransmit a GPS packet\r\n");
					Serial.print("h\t-\tSet high rate mode\r\n");
					Serial.print("l\t-\tSet long range mode\r\n");
					Serial.print("m\t-\tCheck operating mode\r\n");
					Serial.print("c\t-\tConstant Transmit on/off\r\n");
					Serial.print("+\t-\tIncrement LoRa offset\r\n");
					Serial.print("-\t-\tDecrement LoRa offset\r\n");
					Serial.print("?\t-\tShow this menu\r\n");
					break;
					
		default:	// ignore
					break;
	}
	
	return(retval);
}

int LongRangeCommandHandler(uint8_t *cmd,uint16_t cmdptr)
{
	// ignore a single key stroke
	if(cmdptr<=2)	return(0);

#if (DEBUG>0)
	Serial.println((char *)cmd);
#endif
	
	int retval=1;
	
	switch(cmd[1]|0x20)
	{
	
		default:	// ignore
					break;
	}
}

int HighRateCommandHandler(uint8_t *cmd,uint16_t cmdptr)
{
	// ignore a single key stroke
	if(cmdptr<=2)	return(0);

#if (DEBUG>0)
	Serial.println((char *)cmd);
#endif
	
	int retval=1;
	
	switch(cmd[1]|0x20)
	{
	
		default:	// ignore
					break;
	}
}

int now;

void PollLoRa(void)
{
#if 1
	if(LoRaTransmit)
	{
		Serial.println("Starting tx ...");
		
		now=millis();
										
#if 1
		Serial.println("Setting LoRa parameters");
		LoRa.setTxPower(5);
		LoRa.setSpreadingFactor(11);
		LoRa.setSignalBandwidth(125E3);
		LoRa.setCodingRate4(8);
		LoRa.setFrequency(434.65E6);
#endif
					
		LoRa.beginPacket(false);
		LoRa.write(TxPacket,TxPacketLength);
		LoRa.endPacket();

		Serial.print("\tblocking lora tx done in ");
		Serial.print(millis()-now);
		Serial.println(" ms");
					
		LoRaTransmit=0;
	}
	else
	{
		if(millis()>(now+5000))
		{
			strcpy((char *)TxPacket,"Hello, world!");
			TxPacketLength=strlen((char *)TxPacket);
			LoRaTransmit=1;
		}
	}
#endif	
	
//	Serial.println(LoRa.random());

}

