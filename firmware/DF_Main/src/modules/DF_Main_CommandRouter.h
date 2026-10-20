#pragma once

#include <Arduino.h>


void analyzeLedControlCommand(String msg);


void sendRodVrtCmd(int act, unsigned int cnt, unsigned int ontime, unsigned int offtime);
void sendRodBtnLedCmd(int posi, int act, unsigned int cnt, unsigned int ontime, unsigned int offtime);


void analyzeDeviceInfoRequest(String msg);


void apAlive_Check();
void send2RodAlive_CntCheck();
void stopRodRegistration();
void startRodRegistration();
void rodRegi3secToStart();
void processRodRegistration();
void cancelRodRegistration();
void analyzeRodRegistration(String msg);
void executeRodRegistration(String msg);
void tmInput_BoardElecMonitorExec(short _exec);
void tmInput_BbnMotorMonitorExec(short _exec);
void analyzeTestModuleInputTestRequest(String msg);
void analyzeTestModuleOutputTestRequest(String msg);
void analyzeApInfo(String msg);
void analyzeMainAlive(String msg);

extern unsigned int cnt;
extern unsigned int reqCnt;
extern byte nvm_fish_level;
