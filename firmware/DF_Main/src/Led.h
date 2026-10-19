 
 
 

#pragma once

#include "arduino.h"



 
#define CENT_HZ 		22000		 
#define OTHER_HZ 		22000		 
#define LED_RESOL_8		8			 
#define LED_RESOL_10		10			 


 

 
#define mapled(a) map(a, 0,255, 0,1023)		 

 
#define ledOut(a,b)	analogWrite(a,(b*ledLight/100))		 

 
enum _ledPos
{
	LEDPOS_CENT_R = 0,
	LEDPOS_CENT_G,
	LEDPOS_CENT_B,
	LEDPOS_CENT_RGB,	 
	LEDPOS_BTM,			 
	LEDPOS_LEFT,		 
	LEDPOS_RIGHT,		 

	LEDPOS_MAX,			 
};

class Led
{
  public:
    Led();

    void init();
    void on(unsigned int ledpos, int _pwm);
    void RGBon(unsigned int ledpos, int Rval, int Gval, int Bval);
	void off(unsigned int ledpos);
	void blink(unsigned int ledpos, int btime, int bCnt);

	String fillZero(int _count);

    void LedCallback(void (*ledCallback)(String));	

  private:
   










    int blinkCnt;
  
};


 
extern unsigned long curr_ms_tick;
 
extern unsigned int ledLight;
