 
 
 

#pragma once

#include "arduino.h"

class ServoMotor
{
  public:
    ServoMotor();
    void init();

    void setAngle(String _data);
    void setAngle(int _val);
    void setAngleDefault();
    int maxAdjAngle = 90;
  private:
    int defaultAngle = 90;
};

 
extern unsigned long curr_ms_tick;			 
extern String STX_SERVO_MOTOR;

