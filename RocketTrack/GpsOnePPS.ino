
#include "HardwareAbstraction.h"

int firsttime=1;

int ticksemaphore=0;
unsigned long int ticktime_micros=0;
unsigned long int ticktime_millis=0;

long int millis_offset=0;
long int micros_offset=0;

#define NUMPTS	1

void SetupOnePPS(void)
{
	pinMode(GPS_1PPS,INPUT_PULLDOWN);
	attachInterrupt(GPS_1PPS,OnePPSInterrupt,RISING);
}

void OnePPSInterrupt(void)
{
	ticktime_micros=micros();
	ticktime_millis=millis();
	ticksemaphore=1;
}

void PollOnePPS(void)
{
	if(ticksemaphore)
	{
#if 0
		Serial.print("tick\r\n");
#endif
				
		OnePPS_adjust();
		ticksemaphore=0;
	}
}

void OnePPS_adjust(void)
{
	static unsigned long int last_millis;
	static unsigned long int last_micros;

	static long int rect[NUMPTS]={0};
	static int cnt=0;
		
	if(firsttime)
	{
		last_millis=ticktime_millis-1000;
		last_micros=ticktime_micros-1000000;
		firsttime=0;
	}

	// adjust the micros() timer
	
	int slippage=(long int)ticktime_micros-(long int)last_micros-1000000;
	
	// this is to cope with potentially missing a pulse or two if the GPS
	// has lost lock
	
	while(slippage<-10000)	slippage+=1000000;
	while(slippage>10000)	slippage-=1000000;
	
	rect[cnt]=slippage;

#if 1
	Serial.print(slippage);
	Serial.print("\t\t");
#endif
	
	long int mean=0;
	for(int i=0;i<NUMPTS;i++)
		mean+=rect[i];
	
	mean/=NUMPTS;
	micros_offset-=mean;

	cnt++;
	if(cnt>=NUMPTS)
		cnt=0;

#if 1
	Serial.print(micros_offset);
	Serial.print("\t\t");
#endif
	
	last_micros=ticktime_micros;
	
	// adjust the millis() timer
	
#if 0
//	Serial.print(ticktime_millis-last_millis-1000);
//	Serial.print("\t");
#endif
	
//	millis_offset-=ticktime_millis-last_millis-1000;

	last_millis=ticktime_millis;

	Serial.print("tick\r\n");
}

unsigned long int millis_1pps(void)
{
	return(millis()-millis_offset);
}

unsigned long int micros_1pps(void)
{
	return(micros()+micros_offset);
}

