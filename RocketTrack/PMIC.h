
#pragma once

#include "AXP202.h"

int SetupPMIC(void);
void PollPMIC(void);
int PMICCommandHandler(uint8_t *cmd,uint16_t cmdptr);
void ControlLED(axp_chgled_mode_t Mode);

extern bool short_button_press;
extern bool long_button_press;
