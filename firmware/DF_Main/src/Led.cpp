 
 
 

#include "Common.h"
#include "Led.h"


void (*_ledCallback)(String);

Led::Led()
{

}

void Led::init()
{

	 
  	analogWrite(LED_CENT_R_PIN,LED_OFF_DUTY);			 
  	analogWrite(LED_CENT_G_PIN,LED_OFF_DUTY);			 
  	analogWrite(LED_CENT_B_PIN,LED_OFF_DUTY);			 
  	analogWrite(LED_BTM_PIN,LED_OFF_DUTY);			 

  		 
  		 
	

	int ledoff = mapled(LED_OFF_DUTY);

	LogPrintln(" LG] LEDOFF LedOffDuty: " + String(ledoff) );

}


 
void Led::LedCallback(void (*ledCallback)(String))
{
  _ledCallback = ledCallback;
}


 




void Led::on(unsigned int ledPos, int _duty)
{


	if(LEDPOS_MAX <= ledPos)
	{
		LogPrintln(" LG] NGLED " + String(ledPos));
		return;
	}
	else
	{
		switch(ledPos)
		{
			case LEDPOS_CENT_R:
				ledOut(LED_CENT_R_PIN, _duty);
				break;
			
			case LEDPOS_CENT_G:
				ledOut(LED_CENT_G_PIN, _duty);
				break;
			
			case LEDPOS_CENT_B:
				ledOut(LED_CENT_B_PIN, _duty);
				break;
			
			case LEDPOS_BTM:
				ledOut(LED_BTM_PIN, _duty);
				break;
			
			case LEDPOS_LEFT:

					 

			    break;
			  
			case LEDPOS_RIGHT:

				   

			    break;
			  
			default:
				 
				break;
		}
	}


}

void Led::RGBon(unsigned int ledPos, int rVal, int gVal, int bVal)
{
	
	
	if(LEDPOS_MAX <= ledPos)
	{
		LogPrintln(" LG] NGLED " + String(ledPos));
		return;
	}
	else
	{
		switch(ledPos)
		{
			case LEDPOS_CENT_RGB:
				ledOut(LED_CENT_R_PIN, rVal);
				ledOut(LED_CENT_G_PIN, gVal);
				ledOut(LED_CENT_B_PIN, bVal);
				break;
				
			case LEDPOS_BTM:
				ledOut(LED_BTM_PIN, bVal);
				break;
			
			default:
				 
				break;
		}
	}


}

void Led::off(unsigned int ledPos)
{
	
	
	if(LEDPOS_MAX <= ledPos)
	{
		LogPrintln(" LG] LEDNG " + String(ledPos));
		return;
	}
	else
	{
		switch(ledPos)
		{
			case LEDPOS_CENT_RGB:
				ledOut(LED_CENT_R_PIN, LED_OFF_DUTY);
				ledOut(LED_CENT_G_PIN, LED_OFF_DUTY);
				ledOut(LED_CENT_B_PIN, LED_OFF_DUTY);
				break;
			case LEDPOS_BTM:
				ledOut(LED_BTM_PIN, LED_OFF_DUTY);
				break;
			
			case LEDPOS_LEFT:

				   
			  
				break;
			  
			case LEDPOS_RIGHT:
				
				   

				break;
			  
			default:
				 
				break;
		}
	}


}

void Led::blink(unsigned int ledPos, int time, int bCnt)
{
	 
}


