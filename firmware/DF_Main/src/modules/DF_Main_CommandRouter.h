#pragma once

#include <Arduino.h>


void analyzeLedControlCommand(String msg);


void sendRodVrtCmd(int act, unsigned int cnt, unsigned int ontime, unsigned int offtime);
void sendRodBtnLedCmd(int posi, int act, unsigned int cnt, unsigned int ontime, unsigned int offtime);


void anaDeviceInfoReq(String msg);


void apAlive_Check();
void send2RodAlive_CntCheck();
void rodRegistToStop();
void rodRegistToStart();
void rodRegi3secToStart();
void rodRegistToControl();
void rodRegistCancel();
void anaRodRegist(String msg);
void rodRegistExec(String msg);
void tmInput_BoardElecMonitorExec(short _exec);
void tmInput_BbnMotorMonitorExec(short _exec);
void anaTmInputTestReq(String msg);
void anaTmOutputTestReq(String msg);
void anaApInfo(String msg);
void anaMainAlive(String msg);

extern unsigned int cnt;
extern unsigned int reqCnt;
extern byte nvm_fish_level;
