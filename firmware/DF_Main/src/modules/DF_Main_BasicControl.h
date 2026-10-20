#pragma once

#include <Arduino.h>

void analyzeDeviceControlRequest(String msg);
void analyzeRestDistance(String msg);
void analyzeMainMotorControl( String msg );
void analyzeBobbinControlCheck(String msg);
void analyzeBobbinControl(String msg);
void checkRodTimer();
void set_rod_conn_status(unsigned int sts);

