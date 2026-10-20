#pragma once

#include <Arduino.h>

void t100ms_Process();
void t500ms_Process();
void t1sec_Process();
void rodLed1SecControl();
void run_timer();
void ioPinSetting();
void checkFileSystem();

extern unsigned int leftButtonDownCount;
extern unsigned int AddrWrite_LedBlink;
extern unsigned int system_delay_cnt;
extern int resp_imu;
