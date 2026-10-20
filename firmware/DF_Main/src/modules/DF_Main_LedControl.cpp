
#include "../DF_Main_Internal.h"
#include "DF_Main_LedControl.h"
void initializeLedControlValues(int Act)
{
	int i=0;

	memset(ledControl, 0, sizeof(ledControl));
	for(i=0; i<LED_IDX_MAX; i++)
	{
		if (Act) {
			ledControl[i].previousControl = 	initialLedControl[i].previousControl;
			ledControl[i].control = 		initialLedControl[i].control;
		}
		ledControl[i].previousControl = 	initialLedControl[i].previousControl;
		ledControl[i].control = 		initialLedControl[i].control;
		ledControl[i].colorNo = 	initialLedControl[i].colorNo;
		ledControl[i].colorMany = 	initialLedControl[i].colorMany;
		ledControl[i].bTime = 		initialLedControl[i].bTime;
		ledControl[i].dTime = 		initialLedControl[i].dTime;
		ledControl[i].bStep = 		initialLedControl[i].bStep;
		ledControl[i].dStep = 		initialLedControl[i].dStep;
		ledControl[i].dColorCnt = 	initialLedControl[i].dColorCnt;
		ledControl[i].dReqCnt = 	initialLedControl[i].dReqCnt;
		ledControl[i].rsv_e = 		initialLedControl[i].rsv_e;
	}

	for(i=LED_IDX_CENT; i<LED_IDX_MAX; i++)
	{
		switch(ledControl[i].control)
		{
			case 0:  
				ledControl_OffStart(i);
				break;
			case 1:  
				ledControl_OnStart(i);
				break;
			case 2:  
				ledControl_BlinkStart(i);
				break;
			case 3:  
				ledControl_DimmingStart(i);
				break;
		}
	}

	ledControlMode = 1;

	ledLight = 100;	 

}

void ledControl_OffStart(unsigned int pos)
{
	if(0==pos) { return;}

	ledControl[pos].bStep = 0;
	ledControl[pos].dStep = 0;

	extLed.off(pos+2);
}

void ledControl_OnStart(unsigned int pos)
{
	if(0==pos) { return;}

	ledControl[pos].bStep = 0;
	ledControl[pos].dStep = 0;

	if(LED_IDX_CENT == pos)
	{
		setRGBcolor(pos, (int)ledControl[pos].colorNo);
		extLed.RGBon(LEDPOS_CENT_RGB, sRGB[pos].r, sRGB[pos].g, sRGB[pos].b );	
	}
	else
	{
		extLed.on(pos+2, (int)LED_MAX_DUTY);
	}

}

void ledControl_BlinkStart(unsigned int pos)
{
	if(0==pos) { return;}

	ledControl[pos].dStep = 0;

	setRGBcolor((int)pos, (int)ledControl[pos].colorNo );

	ledControl[pos].bStep = 10;	 
}

void executeLedControlBlink(int pos)
{
	static unsigned long _to[LED_IDX_MAX] ={0,0,0,0,0};
	static unsigned int _oldStep[LED_IDX_MAX] = {0,0,0,0,0};

	switch(ledControl[pos].bStep)
	{
		case 0:	 
			break;

		case 10:	 

			if(LED_IDX_CENT == pos)
			{
				extLed.RGBon((pos+2), sRGB[pos].r, sRGB[pos].g, sRGB[pos].b );
			}
			else
			{
				extLed.on((pos+2), (int)LED_MAX_DUTY);
			}

			setTO(_to[pos]);
			ledControl[pos].bStep = 11;	 
			break;
		case 11:			 
			if(checkTO(_to[pos], ledControl[pos].bTime))
			{
				ledControl[pos].bStep = 20;
			}
			break;

		case 20:		 
			extLed.off(pos+2);

			setTO(_to[pos]);
			ledControl[pos].bStep = 21;	 

			break;

		case 21:	 
			if(checkTO(_to[pos], ledControl[pos].bTime))
			{
				ledControl[pos].bStep = 10;
			}
			break;

		case 30:
			break;
		case 40:
			break;

		case 90:
			break;

		default:
			break;
	}
}

void ledControl_DimmingStart(unsigned int pos)
{
	if(0==pos) { return;}

	ledControl[pos].bStep = 0;

	if(7 > ledControl[pos].colorMany)
	{
		ledControl[pos].dColorCnt = COLOR_RED;				 
	}
	else
	{
		ledControl[pos].dColorCnt = COLOR_WHITE; 			 
	}
	ledControl[pos].dReqCnt = (ledControl[pos].dTime/LED_CONTROL_10MS/2);	 
	setRGBcolor((int)pos, (int)ledControl[pos].dColorCnt );

	ledControl[pos].dStep = 10;	 
}

void executeLedControlDimming(int pos)
{
	static unsigned long _to[LED_IDX_MAX] ={ 0,0,0,0,0 };
	static unsigned int _oldStep[LED_IDX_MAX] = { 0,0,0,0,0 };
	static unsigned int _cnt[LED_IDX_MAX] = { 0,0,0,0,0 };

	switch(ledControl[pos].dStep)
	{
		case 0:  
			break;

		case 10:	 
			if(LED_IDX_BTM == pos)
			{
				setRGBcolor(pos, COLOR_BLUE);	 
			}
			else
			{
				setRGBcolor(pos, ledControl[pos].dColorCnt);	 

			}

			_cnt[pos] = 0;
			ledControl[pos].dStep = 20;

			break;

		case 20:		 
			_cnt[pos]++;
			if(_cnt[pos] > ledControl[pos].dReqCnt)
			{
				extLed.RGBon((pos+2), sRGB[pos].r, sRGB[pos].g, sRGB[pos].b);	 
				setTO(_to[pos]);
				ledControl[pos].dStep = 21;
			}
			else
			{
				outRGB[pos].r = (sRGB[pos].r) * _cnt[pos] / ledControl[pos].dReqCnt;
				outRGB[pos].g = (sRGB[pos].g) * _cnt[pos] / ledControl[pos].dReqCnt;
				outRGB[pos].b = (sRGB[pos].b) * _cnt[pos] / ledControl[pos].dReqCnt;
				extLed.RGBon((pos+2), outRGB[pos].r, outRGB[pos].g, outRGB[pos].b);
			}
			break;
		case 21:
			if(checkTO(_to[pos], ledDimmingHoldTime[pos]))		 
			{
				ledControl[pos].dStep = 30;
			}
			break;

		case 30:		 
			_cnt[pos]--;
			if(_cnt[pos] <= 0)		 
			{
				extLed.RGBon((pos+2), 0,0,0);	 
				setTO(_to[pos]);
				ledControl[pos].dStep = 31;
			}
			else
			{
				outRGB[pos].r=(sRGB[pos].r) * _cnt[pos] / ledControl[pos].dReqCnt;
				outRGB[pos].g=(sRGB[pos].g) * _cnt[pos] / ledControl[pos].dReqCnt;
				outRGB[pos].b=(sRGB[pos].b) * _cnt[pos] / ledControl[pos].dReqCnt;
				extLed.RGBon((pos+2), outRGB[pos].r, outRGB[pos].g, outRGB[pos].b);
			}
			break;
		case 31:
			if(checkTO(_to[pos], ledDimmingHoldTime[pos]))		 
			{
				ledControl[pos].dStep = 40;
			}
			break;

		case 40:
			ledControl[pos].dColorCnt++;
			if(ledControl[pos].dColorCnt > ledControl[pos].colorMany)
			{
				if(7 > ledControl[pos].colorMany)
				{
					ledControl[pos].dColorCnt = COLOR_RED;
				}
				else
				{
					ledControl[pos].dColorCnt = COLOR_WHITE;
				}
			}
			else
			{
			}
			ledControl[pos].dStep = 10;
			break;

		case 50:
			break;

		default:
			break;
	}

}

unsigned int ext_led_dim_step = 10;
static unsigned int old_ext_led_dim_step = 0x00;

static unsigned long tot_time = 0;
unsigned int cnt = 0;
unsigned int reqCnt = 0;
static unsigned int loopCnt = 0;
static unsigned int outCnt = 0;

static unsigned int bDuty = (LED_MAX_DUTY*10);
static unsigned int updownStep = (1*10);

void old_execCentLed_Dimming_Control(int color, int sec)
{

	switch(ext_led_dim_step)
	{
		case 0:	 
			break;

		case 10:	 
			reqCnt = (sec * 1000) / 2 / 10;	 
			loopCnt = (reqCnt+128)/256;  
			if(1 > loopCnt)
			{
				loopCnt = 1;
			}

			if(256 > reqCnt)
			{
				updownStep = (256*10)/reqCnt;
			}
			else
			{
				updownStep = (1*10);
			}

			bDuty = (0*10);

			ledOut(LED_CENT_R_PIN, 0);
			ledOut(LED_CENT_G_PIN, 0);
			ledOut(LED_CENT_B_PIN, bDuty);	 
			ext_led_dim_step = 20;
			break;

		case 20:		 
			bDuty += updownStep ;
			if((LED_MAX_DUTY*10) < bDuty)		 
			{
				outCnt = 0;
				bDuty = 0;
				ext_led_dim_step = 29;
			}
			else
			{
				ext_led_dim_step = 21;
			}
			ledOut(LED_CENT_B_PIN, (bDuty/10));  
			break;

		case 21:
			outCnt++;
			if( !(outCnt % loopCnt) )
			{
				if(outCnt > reqCnt)			 
				{
					outCnt = 0;
					if( LED_MAX_DUTY*10 < bDuty) bDuty = (LED_MAX_DUTY*10);
					ledOut(LED_CENT_B_PIN, (bDuty/10));	 
					ext_led_dim_step = 29;
				}
				else
				{
					ext_led_dim_step = 20;
				}
			}
			break;

		case 29:
			outCnt++;
			if(outCnt > ledoffHoldTimeCnt)
			{
				if( LED_MAX_DUTY*10 < bDuty) bDuty = (LED_MAX_DUTY*10);
				outCnt = 0;
				ext_led_dim_step = 30;
			}
			break;

		case 30:		 
			bDuty -= updownStep;
			if((0*10) > bDuty)		 
			{
				bDuty = (0*10);
				outCnt = 0;
				ext_led_dim_step = 32;		 
			}
			else
			{
				ext_led_dim_step = 31;
			}
			ledOut(LED_CENT_B_PIN, (bDuty/10));	 
			break;

		case 31:
			outCnt++;
			if( !(outCnt % loopCnt) )
			{
				if(outCnt > reqCnt)		 
				{
					bDuty = (0*10);
					outCnt = 0;
					ext_led_dim_step = 32;	 
				}
				else
				{
					ext_led_dim_step = 30;
				}
			}
			break;

		case 32:		 
			outCnt++;
			if(outCnt > ledoffHoldTimeCnt)
			{
				if( 0*10 > bDuty) bDuty = (0*10);
				outCnt = 0;
				ext_led_dim_step = 20;
			}
			break;

		default:
			break;
	}
}

void extLed_Off(unsigned int idx)
{
	switch(idx)
	{
		case LED_IDX_CENT:
		case LED_IDX_BTM:
			extLed.off((idx+2));
			break;

		case LED_IDX_LEFT:
		case LED_IDX_RIGHT:
			break;

		default:
			break;
	}
}

void extLed_On(unsigned int idx)
{
	unsigned int color;

	switch(idx)
	{
		case LED_IDX_CENT:

			color = ledControl[idx].colorNo;
			if(COLOR_MAGENTA < color) { color = COLOR_WHITE; }	 
			setRGBcolor((int)idx, (int)color);
			extLed.RGBon((idx+2), sRGB[idx].r, sRGB[idx].g, sRGB[idx].b);
			break;

		case LED_IDX_BTM:
			extLed.on((idx+2), (int)LED_MAX_DUTY);	 
			break;

		case LED_IDX_LEFT:
		case LED_IDX_RIGHT:
			break;

		default:
			break;
	}
}

void extLed_Control() 
{
	unsigned int pos=0;

	if(PWR_ON == pwrMode)
	{

		if(ledControlMode)
		{

			for(pos=LED_IDX_CENT; pos<LED_IDX_MAX; pos++)
			{
				if(LED_CONTROL_BLINK == ledControl[pos].control)		 
				{

					executeLedControlBlink(pos);
				}
				else if(LED_CONTROL_DIMMING == ledControl[pos].control)	 
				{

					{

						executeLedControlDimming(pos);		 
					}
				}
				else	 
				{
					if(ledControl[pos].previousControl != ledControl[pos].control)
					{
						if(LED_CONTROL_OFF == ledControl[pos].control)	 
						{
							extLed_Off(pos);
						}
						else if(LED_CONTROL_ON == ledControl[pos].control)	 
						{
							extLed_On(pos);
						}
						ledControl[pos].previousControl = ledControl[pos].control;
					}
					else
					{

					}
				}
			}
		}
	}

}
