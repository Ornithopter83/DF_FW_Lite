 
 
 

#pragma once

#include "arduino.h"


#define DIVISION_VAL	2
#define ZERO_VAL		0

#define MIN_COUNT    	0 	 
#define MAX_COUNT    6000	 

class Encoder
{
  public:
    Encoder();

    void init();
    void rotate();

    void setRotateCallback(void (*rotateCallback)(int _dir, int _cnt));

	void setBbnCnt(unsigned int _cnt);
	void setLmCnt(unsigned int _cnt);

	int bbnDir = BBN_ENC_CW;
	 
	unsigned int bbnCnt = (WIRE_HOME_BBN_CNT);			 
	unsigned int lmCnt = 1;

  private:
    unsigned int count = (unsigned int)(WIRE_HOME_BBN_CNT);
	
    int last_ACLK;
    int last_BDT;
	int last_lmEnc;
    int current_ACLK;
    int current_BDT;
    int current_lmEnc;
    String oldStr;

    String fillZero(unsigned int _count);
     
};

 
extern unsigned long curr_ms_tick;


