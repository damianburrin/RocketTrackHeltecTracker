
/*---------------------------------------------------*\
|                                                     |
| LoRa radio code, for downlink, uplink and repeating |
|                                                     |
| Messages can be timed using a GPS reference, to     |
| comply with the TDMA timing requirements.           |
|                                                     |
| Connections:                                        |
|                                                     |
|               Arduino  X - RFM98W DIO5              |
|               Arduino  X - RFM98W DIO0              |
|                                                     |
|               Arduino  X  - RFM98W NSS              |
|               Arduino 11 - RFM98W MOSI              |
|               Arduino 12 - RFM98W MISO              |
|               Arduino 13 - RFM98W CLK               |
|                                                     |
\*---------------------------------------------------*/

#include <SPI.h>
#include <string.h>

#include "Packetisation.h"

// RFM98 registers

#define REG_FIFO                    0x00
#define REG_OPMODE                  0x01
#define REG_FIFO_ADDR_PTR           0x0D 
#define REG_FIFO_TX_BASE_AD         0x0E
#define REG_FIFO_RX_BASE_AD         0x0F
#define REG_FIFO_RX_CURRENT_ADDR    0x10
#define REG_IRQ_FLAGS_MASK          0x11
#define REG_IRQ_FLAGS               0x12
#define REG_RX_NB_BYTES             0x13
#define REG_MODEM_CONFIG            0x1D
#define REG_MODEM_CONFIG2           0x1E
#define REG_MODEM_CONFIG3           0x26
#define REG_PREAMBLE_MSB            0x20
#define REG_PREAMBLE_LSB            0x21
#define REG_PAYLOAD_LENGTH          0x22
#define REG_HOP_PERIOD              0x24
#define REG_FREQ_ERROR              0x28
#define REG_DETECT_OPT              0x31
#define	REG_DETECTION_THRESHOLD     0x37
#define REG_DIO_MAPPING_1           0x40
#define REG_DIO_MAPPING_2           0x41

// FSK stuff

#define REG_PREAMBLE_MSB_FSK        0x25
#define REG_PREAMBLE_LSB_FSK        0x26
#define REG_PACKET_CONFIG1          0x30
#define REG_PAYLOAD_LENGTH_FSK      0x32
#define REG_FIFO_THRESH             0x35
#define REG_FDEV_MSB                0x04
#define REG_FDEV_LSB                0x05
#define REG_FRF_MSB                 0x06
#define REG_FRF_MID                 0x07
#define REG_FRF_LSB                 0x08
#define REG_BITRATE_MSB             0x02
#define REG_BITRATE_LSB             0x03
#define REG_IRQ_FLAGS2              0x3F

// MODES

#define RF98_MODE_RX_CONTINUOUS     0x85
#define RF98_MODE_TX                0x83
#define RF98_MODE_SLEEP             0x80
#define RF98_MODE_STANDBY           0x81

#define PAYLOAD_LENGTH              255

// Modem Config 1

#define EXPLICIT_MODE               0x00
#define IMPLICIT_MODE               0x01

#define ERROR_CODING_4_5            0x02
#define ERROR_CODING_4_6            0x04
#define ERROR_CODING_4_7            0x06
#define ERROR_CODING_4_8            0x08

#define BANDWIDTH_7K8               0x00
#define BANDWIDTH_10K4              0x10
#define BANDWIDTH_15K6              0x20
#define BANDWIDTH_20K8              0x30
#define BANDWIDTH_31K25             0x40
#define BANDWIDTH_41K7              0x50
#define BANDWIDTH_62K5              0x60
#define BANDWIDTH_125K              0x70
#define BANDWIDTH_250K              0x80
#define BANDWIDTH_500K              0x90

// Modem Config 2

#define SPREADING_6                 0x60
#define SPREADING_7                 0x70
#define SPREADING_8                 0x80
#define SPREADING_9                 0x90
#define SPREADING_10                0xA0
#define SPREADING_11                0xB0
#define SPREADING_12                0xC0

#define CRC_OFF                     0x00
#define CRC_ON                      0x04

// POWER AMPLIFIER CONFIG

#define REG_PA_CONFIG               0x09
#define PA_MAX_BOOST                0x8F    // 100mW (max 869.4 - 869.65)
#define PA_LOW_BOOST                0x81
#define PA_MED_BOOST                0x8A
#define PA_MAX_UK                   0x88    // 10mW (max 434)
#define PA_OFF_BOOST                0x00
#define RFO_MIN                     0x00

// 20DBm
#define REG_PA_DAC                  0x4D
#define PA_DAC_20                   0x87

// LOW NOISE AMPLIFIER
#define REG_LNA                     0x0C
#define LNA_MAX_GAIN                0x23  // 0010 0011
#define LNA_OFF_GAIN                0x00

typedef enum {lmIdle, lmListening, lmSending} tLoRaMode;

bool LoRaTransmit=0;

uint32_t TXStartTimeMillis;

double lora_frequency=LORA_FREQUENCY;
uint8_t lora_mode=LORA_MODE;

tLoRaMode LoRaMode;
byte currentMode=0x81;
//int TargetID;
//struct TBinaryPacket PacketToRepeat;
//byte SendRepeatedPacket, RepeatedPacketType=0;
int ImplicitOrExplicit;
//int GroundCount;
//int AirCount;
//int BadCRCCount;

uint8_t TXPacket[MAX_TX_PACKET_LENGTH];
uint8_t TxPacketLength;

uint16_t TXPacketCounter=0;

uint32_t LastLoRaTX=0;
//unsigned long TimeToSendIfNoGPS=0;
//int CallingCount=0;
//int RTTYCount=0;
//int InRTTYMode=0;
//int SendingRTTY=0;
//int RTTYIndex, RTTYMask, RTTYLength;
//int FSKBitRate, FSKOverSample, RTTYBitLength;

void SetupLoRa(void)
{
	setupRFM98(LORA_FREQUENCY,LORA_MODE);
	
/*
	if (LORA_RTTY_BAUD == 50)
	{
		FSKBitRate=40000;
		FSKOverSample=2;
		RTTYBitLength=7;
	}
	else
	{
		// 300 baud
		FSKBitRate=13333;
		FSKOverSample=1;
		RTTYBitLength=8;
	}
*/
}

void setupRFM98(double Frequency,int Mode)
{
	int ErrorCoding;
	int Bandwidth;
	int SpreadingFactor;
	int LowDataRateOptimize;
	int PayloadLength;
	
	// initialize the pins
#ifdef LORA_RESET
	Serial.println("Resetting LoRa Module ...");
	pinMode(LORA_RESET,OUTPUT);
	digitalWrite(LORA_RESET,LOW);
	delay(10);
	digitalWrite(LORA_RESET,HIGH);
	delay(1000);          // Module needs this before it's ready on these boards (slow power up ?)
	Serial.println("Reset LoRa Module");
#endif
	
	pinMode(LORA_NSS,OUTPUT);
	pinMode(LORA_DIO0,INPUT);

	// SPI.begin();
	SPI.begin(SCK,MISO,MOSI,LORA_NSS);
	
	// LoRa mode 
	setLoRaMode();

	// Frequency
	setFrequency(Frequency+LORA_OFFSET/1000.0);

	// LoRa settings for various modes.  We support modes 2 (repeater mode), 1 (normally used for SSDV) and 0 (normal slow telemetry mode).
	
	if(Mode==5)			{	ImplicitOrExplicit=EXPLICIT_MODE;	ErrorCoding=ERROR_CODING_4_8;	Bandwidth=BANDWIDTH_41K7;	SpreadingFactor=SPREADING_11;	LowDataRateOptimize=0;		}
	else if(Mode==2)	{	ImplicitOrExplicit=EXPLICIT_MODE;	ErrorCoding=ERROR_CODING_4_8;	Bandwidth=BANDWIDTH_62K5;	SpreadingFactor=SPREADING_8;	LowDataRateOptimize=0;		}
	else if(Mode==1)	{	ImplicitOrExplicit=IMPLICIT_MODE;	ErrorCoding=ERROR_CODING_4_5;	Bandwidth=BANDWIDTH_20K8;	SpreadingFactor=SPREADING_6;	LowDataRateOptimize=0;		}
	else				{	ImplicitOrExplicit=EXPLICIT_MODE;	ErrorCoding=ERROR_CODING_4_8;	Bandwidth=BANDWIDTH_20K8;	SpreadingFactor=SPREADING_11;	LowDataRateOptimize=0x08;	}
	
	writeRegister(REG_MODEM_CONFIG,ImplicitOrExplicit|ErrorCoding|Bandwidth);
	writeRegister(REG_MODEM_CONFIG2,SpreadingFactor|CRC_ON);
	writeRegister(REG_MODEM_CONFIG3,0x04|LowDataRateOptimize);	// 0x04: AGC sets LNA gain
	
	// writeRegister(REG_DETECT_OPT, (SpreadingFactor == SPREADING_6) ? 0x05 : 0x03);					// 0x05 For SF6; 0x03 otherwise
	writeRegister(REG_DETECT_OPT,(readRegister(REG_DETECT_OPT)&0xF8)|((SpreadingFactor==SPREADING_6)?0x05:0x03));  // 0x05 For SF6; 0x03 otherwise
	
	writeRegister(REG_DETECTION_THRESHOLD,(SpreadingFactor==SPREADING_6)?0x0C:0x0A);		// 0x0C for SF6, 0x0A otherwise  
	
	writeRegister(REG_PAYLOAD_LENGTH,PayloadLength);
	
	// Change the DIO mapping to 01 so we can listen for TxDone on the interrupt
	writeRegister(REG_DIO_MAPPING_1,0x40);
	writeRegister(REG_DIO_MAPPING_2,0x00);
	
	// Go to standby mode
	setMode(RF98_MODE_STANDBY);
	
	Serial.println("Setup Complete");
}

void setFrequency(double Frequency)
{
	unsigned long FrequencyValue;
	
	Serial.print("Frequency is ");
	Serial.println(Frequency);
	
	Frequency=Frequency*7110656/434;
	FrequencyValue=(unsigned long)(Frequency);
	
	Serial.print("FrequencyValue is ");
	Serial.println(FrequencyValue);
	
	// Set frequency
	writeRegister(0x06,(FrequencyValue>>16)&0xFF);
	writeRegister(0x07,(FrequencyValue>>8)&0xFF);
	writeRegister(0x08,FrequencyValue&0xFF);
}

void setLoRaMode()
{
	Serial.println("Setting LoRa Mode");
	
	setMode(RF98_MODE_SLEEP);
	writeRegister(REG_OPMODE,0x80);
}

/////////////////////////////////////
//    Method:   Change the mode
//////////////////////////////////////

void setMode(byte newMode)
{
	if(newMode==currentMode)
		return;  
	
	Serial.printf("Set LoRa Mode %d\n",newMode);
	
	switch(newMode) 
	{
		case RF98_MODE_TX:				writeRegister(REG_LNA,LNA_OFF_GAIN);		// TURN LNA OFF FOR TRANSMIT
										writeRegister(REG_PA_CONFIG,PA_MAX_UK);
										writeRegister(REG_OPMODE,newMode);
										TXStartTimeMillis=millis();
										currentMode=newMode; 
										break;
		
		case RF98_MODE_RX_CONTINUOUS:	writeRegister(REG_PA_CONFIG,PA_OFF_BOOST);	// TURN PA OFF FOR RECEIVE??
										writeRegister(REG_LNA,LNA_MAX_GAIN);  		// MAX GAIN FOR RECEIVE
										writeRegister(REG_OPMODE,newMode);
										currentMode=newMode;	 
										break;
		
		case RF98_MODE_SLEEP:			writeRegister(REG_OPMODE,newMode);
										currentMode=newMode; 
										break;
		
		case RF98_MODE_STANDBY:			writeRegister(REG_OPMODE,newMode);
										currentMode=newMode; 
										break;
		
		default: 						// do nothing
										return;
	} 
	
	if(newMode!=RF98_MODE_SLEEP)
		delay(10);
}

byte readRegister(byte addr)
{
	select();
	SPI.transfer(addr&0x7F);
	byte regval=SPI.transfer(0);
	unselect();
	
	printf("RD Reg %02X=%02X\n",addr,regval);
	return regval;
}

void writeRegister(byte addr,byte value)
{
	select();
	SPI.transfer(addr|0x80); // OR address with 10000000 to indicate write enable;
	SPI.transfer(value);
	unselect();
	
	printf("WR Reg %02X=%02X\n",addr,value);
}

void select() 
{
	digitalWrite(LORA_NSS,LOW);
}

void unselect() 
{
	digitalWrite(LORA_NSS,HIGH);
}

int LoRaIsFree(void)
{
	if(		(LoRaMode!=lmSending)
		||	digitalRead(LORA_DIO0)	)
	{
		// Either not sending, or was but now it's sent.  Clear the flag if we need to
		if(LoRaMode==lmSending)
		{
			// Clear that IRQ flag
			writeRegister(REG_IRQ_FLAGS,0x08); 
			LoRaMode=lmIdle;
			
			uint32_t TXBurstTime=millis()-TXStartTimeMillis;
			Serial.print("Tx burst time=");	Serial.print(TXBurstTime);	Serial.println(" ms");
		}
	}
	
	return 0;
}

void SendLoRaPacket(unsigned char *buffer,int Length)
{
	int i;
	
	LastLoRaTX=millis();
	
	setupRFM98(LORA_FREQUENCY,LORA_MODE);
	
	Serial.print("Sending "); Serial.print(Length);	Serial.println(" bytes");
	
	setMode(RF98_MODE_STANDBY);
	
	// 01 00 00 00 maps DIO0 to TxDone
	writeRegister(REG_DIO_MAPPING_1,0x40);
	
	// Update the address ptr to the current tx base address
	writeRegister(REG_FIFO_TX_BASE_AD,0x00);
	writeRegister(REG_FIFO_ADDR_PTR,0x00); 
	
	if(ImplicitOrExplicit==EXPLICIT_MODE)
	{
		writeRegister(REG_PAYLOAD_LENGTH,Length);
	}
	
	select();
	
	// tell SPI which address you want to write to
	SPI.transfer(REG_FIFO|0x80);

	// loop over the payload and put it on the buffer 
	for(i=0;i<Length;i++)
	{
		printf("WR Reg %02X=%02X\n",REG_FIFO|0x80,buffer[i]);
		SPI.transfer(buffer[i]);
	}
	
	unselect();
	
	// go into transmit mode
	setMode(RF98_MODE_TX);
	
	LoRaMode=lmSending;
}

int LORACommandHandler(uint8_t *cmd,uint16_t cmdptr)
{
#if (DEBUG>0)
	Serial.println((char *)cmd);
#endif
	
	int retval=1;
	
	switch(cmd[1]|0x20)
	{
		case 't':	// transmit a burst
					
					Serial.println("Transmitting LoRa packet");
					
					// $$LORA1,108,20:30:39,51.95027,-2.54445,00141,0,0,11*9B74
					
					memcpy(TXPacket,"$$Hello world!\r\n",16);
					TxPacketLength=16;
					LoRaTransmit=1;
					break;
		
		case 'l':	// long range mode
					
					Serial.println("Long range mode");
					lora_mode=0;
					
					break;
		
		case 'h':	// high rate mode
					
					Serial.println("High rate mode");
					lora_mode=2;
					
					break;
		
		case 'm':	// any mode
					
					Serial.print("Setting LoRa Mode to ");
					Serial.println(cmd[2]-0x30);
					lora_mode=cmd[2]-0x30;
		
		default:	// ignore
					break;
	}
	
	return(retval);
}

void PollLoRa(void)
{
	// wait for DIO0 to indicate end of transmission
	
	if(digitalRead(LORA_DIO0))
	{
		// Clear that IRQ flag
		writeRegister(REG_IRQ_FLAGS,0x08); 
		LoRaMode=lmIdle;
		
		uint32_t TXBurstTime=millis()-TXStartTimeMillis;
		Serial.print("Tx burst time=");	Serial.print(TXBurstTime);	Serial.println(" ms");	
	}
	
	if(LoRaTransmit)
	{
		if(TxPacketLength>0)
		{
			setupRFM98(lora_frequency,lora_mode);
			SendLoRaPacket(TXPacket,TxPacketLength);
		}
		
		LoRaTransmit=0;
	}
}

