
#pragma once

int SetupLoRa(void);

void PollLoRa(void);

int LORACommandHandler(uint8_t *cmd,uint16_t cmdptr);

extern double lora_freq;
extern char lora_mode[];
extern int lora_crc;

extern int hr_bw;
extern int hr_sf;
extern int hr_cr;
extern int hr_period;

extern int lr_bw;
extern int lr_sf;
extern int lr_cr;
extern int lr_period;

extern bool lora_constant_transmit;

extern int tx_active;

