 
 
 

#pragma once

#include "arduino.h"

 





 


























 
extern int mainMode;
extern int oldmainMode;
extern unsigned long curr_ms_tick;
 



 

extern void diagInput_AllOff();
extern void diagOutput_AllOff();
extern void ledOn_MbBtm();
extern void initSet_LedContVal(int Act=1);
extern void motor_AllOff();
 
extern void extLed_controlOff();
extern void reelOut_AllOff();
