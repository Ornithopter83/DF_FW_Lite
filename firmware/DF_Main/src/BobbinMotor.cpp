 
 
 

#include "Common.h"
#include "BobbinMotor.h"

BobbinMotor::BobbinMotor()
{
  
}

 


void BobbinMotor::init()
{ 

	 
  digitalWrite(BBN_BLDC_24VON_PIN, BBN_BLDC_24VOFF);		 

  digitalWrite(BBN_MOT_ON_PIN, BBN_MOT_OFF);		 
  digitalWrite(BBN_MOT_DIR_PIN, BBN_MOT_CW);		 
  
  	 
  ledcSetup(BOBBIN_CH, BLDC_HZ, BLDC_RESOL);		 
  ledcAttachPin(BBN_MOT_PWM_PIN, BOBBIN_CH);				 

   
  ledcWrite(BOBBIN_CH, BOBBIN_MAX_DUTY);					   

	 
	sbbnOut = PWM_MIN_DUTY;

	digitalWrite(BBN_MOT_ON_PIN, BBN_MOT_OFF);		 

}

 





void BobbinMotor::onBldcString(String _msg)
{


  int iCat = _msg.substring(3,(3+2)).toInt(); 		 
  											 
  int iItem = _msg.substring(6,(6+2)).toInt(); 		 
  											 
  int iAct = _msg.substring(9,(9+1)).toInt(); 		 
  											 
  int iPara = _msg.substring(11,(11+3)).toInt(); 	 

  if( (3 != iCat) || (3 != iItem))	{ return;	}	 

  switch(iAct)
  {
  	case 0 :	 
		offBldc();
		break;
	case 1 :	 
		onBldc(BBN_MOT_CW, iPara);
		break;
	case 2 :	 
		onBldc(BBN_MOT_CCW, iPara);
		break;
	case 3:		 
		 
		break;
	default:
		 
		break;
  }

}


void BobbinMotor::onBldc(int _dir, int _duty255)
{
	int	inDuty = _duty255;

	if (_duty255 <= 0 || bldcLimitVal <= 0)
	{
		offBldc();
		return;
	}

	String logMsg;

	 
	 
	digitalWrite(BBN_BLDC_24VON_PIN, BBN_BLDC_24VON);

	 
	digitalWrite(BBN_MOT_DIR_PIN, _dir);			 
   
 
	if(PWM_MIN_DUTY > _duty255) _duty255 = PWM_MIN_DUTY;

 
	 
	else if(bldcLimitVal < _duty255) _duty255 = bldcLimitVal;	 
	else { }

	
	 

	 
	sbbnOut = _duty255;

  
  	 
  	outDuty = BOBBIN_MAX_DUTY - (BOBBIN_MAX_DUTY * _duty255 / PWM_ADJ_MAX100);
  	ledcWrite(BOBBIN_CH, outDuty);	   
	 
	if(0 != _duty255)	 
	{
	  	digitalWrite(BBN_MOT_ON_PIN, BBN_MOT_ON); 	   
	}

	 
	 
	logMsg = ((BBN_MOT_CW ==_dir)? "CW_" : "CCW");
	 
	LogPrintln(" LG] BLDCo " + logMsg + "," + String(_duty255) + "/255" + (_duty255 != inDuty ? "in:" + String(inDuty) : "")
		+ ",pwm_cmd:" + String(outDuty) + "/" + String(BOBBIN_MAX_DUTY)
		+ ",hz:" + String(ledcReadFreq(BOBBIN_CH)));


}

void BobbinMotor::offBldc()
{


		digitalWrite(BBN_BLDC_24VON_PIN, BBN_BLDC_24VOFF);		 
  
	digitalWrite(BBN_MOT_ON_PIN, BBN_MOT_OFF);

  		ledcWrite(BOBBIN_CH, BOBBIN_MAX_DUTY);

	 
	sbbnOut = PWM_MIN_DUTY;

   

		LogPrintln(" LG] BLDCo OFF: " + String(BOBBIN_MAX_DUTY));



}



