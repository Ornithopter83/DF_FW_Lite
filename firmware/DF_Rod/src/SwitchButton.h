 

#pragma once

#include "arduino.h"


class SwitchButton
{
  public:
    SwitchButton();
    void init();
    void toggle();

    void setSwitchCallback(void (*switchCallback)(String));
    
  private:
    bool redBtnToggle;		 
    bool blueBtnToggle;		 
     
    bool oldswSetup;
    bool oldswBoot;
};

 
 
extern unsigned int btn_L_DownCnt;

extern unsigned short btnLFLedRed_OutSts;
extern unsigned short btnRTLedBlue_OutSts;

extern unsigned short apType;

 
 
extern int getButtonType();

