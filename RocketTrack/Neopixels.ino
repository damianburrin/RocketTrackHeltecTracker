
bool neopixels_enable=true;
uint32_t NextNeopixels=0;

#include <Adafruit_NeoPixel.h>

//define NeoPixel Pin and Number of LEDs
#define PIN 5
#define NUM_LEDS 3

Adafruit_NeoPixel strip=Adafruit_NeoPixel(NUM_LEDS,PIN,NEO_GRB+NEO_KHZ800);

int SetupNeopixels(void)
{
	if(neopixels_enable)
	{
		strip.begin();
		strip.show();
	}
	
	return(0);
}

void PollNeopixels(void)
{
	if(neopixels_enable)
	{
		if(millis()>=NextLEDs)
		{
			// do something, dunno what just yet
			
			// run the neopixels on a 10Hz cycle
			NextNeopixels=millis()+100L;
		}
	}
}

