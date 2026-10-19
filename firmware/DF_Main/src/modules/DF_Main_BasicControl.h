#pragma once

#include <Arduino.h>

void anaDeviceContReq(String msg);
void anaRestDistance(String msg);
void ana_AngleControl( String msg );
void ana_MainMotControl( String msg );
void ana_BobbinControl_Check(String msg);
void ana_BobbinControl(String msg);
void checkRodTimer();
void set_rod_conn_status(unsigned int sts);
void condition_fw_check_10ms();
