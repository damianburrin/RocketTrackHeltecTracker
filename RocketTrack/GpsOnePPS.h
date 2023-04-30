
#pragma once

extern int ticksemaphore;

void SetupOnePPS(void);
void OnePPS_adjust(void);

unsigned long int millis_1pps(void);
unsigned long int micros_1pps(void);

