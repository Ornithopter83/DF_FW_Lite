#pragma once

#include <Arduino.h>

void imuDataOutCmdSend(int onOff);
void usb5vChange_Check();


void reelAllOff_SendControl();

void t1ms_Process_Exec();
void t10ms_Process_Exec();
void t100ms_Process_Exec();
void t500ms_Process_Exec();
void t1sec_Process_Exec();
void sendCmdSts_MainPwrOn();
void logUartRecvParsing();
void logRecvHandler();
void extIOuartRecvHandler();
void imuInterval_Measure();
void nowRecvHandler();
void uartRecvParsing();
void uartRecvHandler();
String analyzeVerticalMotorCommand(String msg);


String analyzeButtonLedCommand(String msg);


void torqMotor_DefaultOut();


void boardLed_Control();
void read_input_1ms();
void read_input_10ms();
void read_input_100ms();


void subAc_Control_atReset();
short setConfig();
void ioLedOffSetting();
void ioPinSetting();

void IRAM_ATTR ISR_onTimerHandler();
void IRAM_ATTR ISR_encIntrHandle();
void spi_out(int _pin, int _val);
void system_counter();
int getBoardType();

extern int g_ProcStatus;
extern int pwrOn1stCmdSendFlag;
extern int handEncCnt;
extern volatile int rcved_flag;
extern int rcved_Seq;
extern String rcvApMsg;
extern String rcvApMsg_Save;
extern unsigned long pulseWidthMs;
