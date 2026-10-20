 
 
 

#include "Common.h"
#include "SwitchButton.h"


 
 
String PID_SWITCH = "$06";

 
String BLUE_BUTTON_UP = "00000001%";		 
String BLUE_BUTTON_DOWN = "00000010%";		 
String RED_BUTTON_UP = "00000100%";			 
String RED_BUTTON_DOWN = "00001000%";		 

 
void (*_switchCallback)(String);

 
SwitchButton::SwitchButton()
{

}

 
void SwitchButton::init()
{
	 
	pinMode(BTN_RED_PIN, INPUT);
	pinMode(BTN_BLUE_PIN_NEW, INPUT);

	 
	redBtnToggle = digitalRead(BTN_RED_PIN);
	blueBtnToggle = digitalRead(BTN_BLUE_PIN_NEW);
}

 
 
void SwitchButton::setSwitchCallback(void (*switchCallback)(String))
{
  _switchCallback = switchCallback;
}

 
void SwitchButton::toggle()
{
  String str = "";

  bool redBtnState = digitalRead(BTN_RED_PIN);
  if (redBtnState != redBtnToggle)
  {
    if (redBtnState)
    {
       
      str = "21";	 
	  btnRTLedBlue_OutSts = BTN_LED_OFF;
	  digitalWrite(BTN_LED_RT_BLUE_PIN, btnRTLedBlue_OutSts);
    }
    else
    {
       
      str = "20";	 
	  btnRTLedBlue_OutSts = BTN_LED_ON;
	  digitalWrite(BTN_LED_RT_BLUE_PIN, btnRTLedBlue_OutSts);


	}

	if (_switchCallback != NULL)
    {
      _switchCallback(str);
    }
    redBtnToggle = redBtnState;

  }

	 
	bool blueBtnState = digitalRead(BTN_BLUE_PIN_NEW);
  if (blueBtnState != blueBtnToggle)
  {
    if (blueBtnState)
    {
       
      str = "11";	 
      leftButtonDownCount = 0;	 
	  btnLFLedRed_OutSts = BTN_LED_OFF;
	  digitalWrite(BTN_LED_LF_RED_PIN, btnLFLedRed_OutSts);
    }
    else
    {
       
      str = "10";	 
	  leftButtonDownCount = 1;	 
	  btnLFLedRed_OutSts = BTN_LED_ON;
	  digitalWrite(BTN_LED_LF_RED_PIN, btnLFLedRed_OutSts);


	}

	if (_switchCallback != NULL)
    {
      _switchCallback(str);
    }
	
    blueBtnToggle = blueBtnState;
	
  }

   
  if(AP_IS_TM == apType)
  {
  	 
	 
	bool swSetup = digitalRead(SETUP_MODE_PIN);
	if (oldswSetup != swSetup)
	{
	  if (swSetup)	{ str = "80"; }  
	  else			{ str = "81"; }  
	  if (_switchCallback != NULL)
	  {
		_switchCallback(str);
	  }
	  oldswSetup = swSetup;
	}
	 
	bool swBoot = digitalRead(BOOT_MODE_PIN);
	if (oldswBoot != swBoot)
	{
	  if (swBoot)	{ str = "91"; }  
	  else			{ str = "90"; }  
	  if (_switchCallback != NULL)
	  {
		_switchCallback(str);
	  }
	  oldswBoot = swBoot;
	}
	 
  }
}
