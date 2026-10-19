 
 
 

#pragma once

#include "arduino.h"
#include "SPI.h"

#define RECV_MIN  0
#define RECV_MAX  255

#define TORQ_OFF_VALUE  0

 
#define TORQ_00W_FULL	0	 

#define TORQ_10W_FULL	1
#define TORQ_20W_FULL	2
#define TORQ_30W_FULL	3

#define TORQ_10W_LIMIT	4
#define TORQ_20W_LIMIT	5
#define TORQ_30W_LIMIT	6

class Potentiometer
{
  public:
    Potentiometer();

    void init();
    void setValue(String _val);		 
    void setValue(int _val);		 
	void outValue(int _val);		 
    void on(int val);		 

	 
	void respTorqOutVal(int val);

	int torqWattAdj = TORQ_30W_FULL;			 
	 
	
	int setAdjTorq = 0;

	int defaultVal = DEFAULT_TORQ;  

  private:
    void digitalPortWrite(int _pinNum, int _val);
};


 
extern unsigned long curr_ms_tick;

extern String STX_TORQUE_MOTOR;

extern int gameStatus;
extern unsigned int	motTestAct;
extern unsigned int	motTestMainIdx;


