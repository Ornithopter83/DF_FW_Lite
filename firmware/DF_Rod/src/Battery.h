#pragma once

#include "arduino.h"


#define BATTERY_PER_MIN_0		0
#define BATTERY_PER_MIN_1		1
#define BATTERY_PER_LOW		20
#define BATTERY_PER_MAX		99

 
#define BATTERY_MIN_VALUE  2795		 
#define BATTERY_MAX_VALUE  4095		 

 
#define BATTERY_MIN_VALUE_NEW  	2828	 
#define BATTERY_MAX_VALUE_NEW   3546	 

#define BAT_BUF_SIZE 		20		 

#define	BAT_UPDATE_INTVL	(60*1000)		 
#define	BAT_UPDATE_GAP		2				 

class Battery
{
  public:

    Battery();
 
	void init();
     
    void scanAvgInteg();
    void scanKalFilter();

    String getLevel3Char(int _lvl);
    bool isLowLevel();

	int ad_buff[BAT_BUF_SIZE];
	int ad_idx = 0;

	int adc_value;						 
	int mapped_lvl;					 
	long sum_ad;
	int avg_ad;
	int integ_ad;

  	int rest_lvl = -2;
	int old_restLvl = -2;		 
	int	old_dir = 0;					 
	unsigned long last_tick = 9999999;	 
	int	chk_cnt = 2;					 

	String restLvlStr;
	
  private:

    bool lowState = false;

};

 
 
extern int getBattType();

