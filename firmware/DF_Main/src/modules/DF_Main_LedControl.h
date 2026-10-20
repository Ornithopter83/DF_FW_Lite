#pragma once

#include <Arduino.h>

void initializeLedControlValues(int Act);
void ledControl_OffStart(unsigned int pos);
void ledControl_OnStart(unsigned int pos);
void ledControl_BlinkStart(unsigned int pos);
void executeLedControlBlink(int pos);
void ledControl_DimmingStart(unsigned int pos);
void executeLedControlDimming(int pos);
void old_execCentLed_Dimming_Control(int color, int sec);
void extLed_Off(unsigned int idx);
void extLed_On(unsigned int idx);
void extLed_Control();

extern unsigned int ext_led_dim_step;
