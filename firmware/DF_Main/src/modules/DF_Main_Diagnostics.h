#pragma once

#include <Arduino.h>

void mDiag_entry();
void mDiag_exit();
void diagInputMonitor();
void diagInputMotorSensorAllOff();

void diagInput_AllOff();
void diagInput_BoardInAllOff();
void diagInput_BoardInAllOn();
void DiagInput_MotInAllOff();
void DiagInput_MotInAllOn();
void DiagInput_MainEncoderInAllOn();
void DiagInput_MechaInAllOff();
void DiagInput_ElecInAllOff();
void DiagInput_ElecInAllOn();
void diagInput_Cat0(String _msg);
void diagInput_Cat1(String _msg);
void diagInput_Cat3(String _msg);
void diagInput_Cat5(String _msg);
void diagInputTest(String _msg);
void diagOutput_AllOff();
void diagOutput_BoardOutAllOff();
void setRGBcolor(int pos, int Color);
void setOutRGBcolor(int pos, int Color);
void diagBbnMotor_TimeoutStart(unsigned int _time);
void diagBbnMotor_TimeoutControl();
void diagBbnMotor_TokTokEnd();
void diagBbnMotor_TokTokStart(unsigned int _ackKind, unsigned int _cnt);
void diagBbnMotor_TokTokControl(unsigned _opKind);
void diagOut_Cat1(String _msg);
void diagOut_Cat3(String _msg);
void diagOutputTest(String _msg);

void diagOutAllOff();


void subAcOff_WithOtherOff();
int CheckAppCmd();
int CheckPowerSwitch();
void check_PowerSwitch_Change_10ms();

extern int diagOutflag_lmMotTimeOut;
