 
 
 

#pragma once

#include "arduino.h"

 
 






 
 
 

 
#define BOBBIN_MAX_DUTY		1023			 
#define BOBBIN_OFF_DUTY	0


 
#define PWM_MIN_DUTY_PER 1		 
#define PWM_MAX_DUTY_PER 50		 

 
#define PWM_MIN_DUTY_1	1			 
#define PWM_MIN_DUTY	0			 

	#define PWM_MAX_DUTY 255		 

 
 



#define PWM_ADJ_MAX200	(255*100/200)	 
#define PWM_ADJ_MAX150	(255*100/150)	 
#define PWM_ADJ_MAX100	255				 
#define PWM_ADJ_MAX075	(255*125/100)	 
#define PWM_ADJ_MAX050	(255*150/100)	 

class BobbinMotor
{
  public:
    BobbinMotor();
    void init();

    void onBldcString(String _msg);
    void onBldc(int _dir, int _duty);
    void offBldc();
	short sbbnOut = 0;
	
  private:
    int defaultDuty;
	int outDuty;
};

 
extern unsigned long curr_ms_tick;

extern int gameStatus;
extern unsigned int	motTestAct;
extern unsigned int	motTestBbnIdx;

extern String STX_BBN_MOTOR;

extern short bldcLimitVal;		 

