 
 
 

#include "Common.h"
#include "Encoder.h"


void (*_rotateCallback)(int _dir, int _cnt);

Encoder::Encoder()
{

}

void Encoder::init()
{
  last_ACLK = (digitalRead(BBN_ENC_A_CK_PIN)? 1:0);
  last_BDT = (digitalRead(BBN_ENC_B_DT_PIN)? 1:0);
}

 
void Encoder::setRotateCallback(void (*rotateCallback)(int _dir, int _cnt))
{
  _rotateCallback = rotateCallback;
}

 
#define BBN_CW_DIR HIGH

 



void Encoder::rotate()
{
	int changeEnc = 0;
	 

 
	 
	current_ACLK = (digitalRead(BBN_ENC_A_CK_PIN)? 1:0);
	current_BDT = (digitalRead(BBN_ENC_B_DT_PIN)? 1:0);

	 
	 
	if(current_ACLK != last_ACLK)
  	{
		 
  		last_ACLK = current_ACLK;

		 
  		if( ((HIGH == current_ACLK) && ( BBN_CW_DIR == current_BDT ))		 
		 
		)	
		{
			 
			bbnDir = BBN_ENC_CW;
 	    	count--;
			bbnCnt--;
 	  		if (MIN_COUNT > bbnCnt)
    		{
   		    	count = MIN_COUNT;
				bbnCnt = MIN_COUNT;
    		}
			changeEnc = 1;
	  	}

		 
	    else if ( ((HIGH == current_ACLK) && ( BBN_CW_DIR != current_BDT ))		 
    	 
		)
		{
		   
			bbnDir = BBN_ENC_CCW;
    		count++;
	 		bbnCnt++;
			if (MAX_COUNT < bbnCnt  )
 	   		{
 	    		count = MAX_COUNT;
				bbnCnt = MAX_COUNT;
 	   		}
			changeEnc = 1;
    	}
	
	}
  
	 
	if(changeEnc)   
	{
		changeEnc = 0;
		String dirStr = (BBN_ENC_CW == bbnDir)? "CW_" : "CCW";

	}

	 
	if(last_BDT != current_BDT)
	{
    	last_BDT = current_BDT;
	}

}


String Encoder::fillZero(unsigned int _count)
{
  String str;
  if (_count < 10)
  {
    str = "0000" + String(_count);
  }
  else if (_count < 100)
  {
    str = "000" + String(_count);
  }
  else if (_count < 1000)
  {
    str = "00" + String(_count);
  }
  else if (_count < 10000)
  {
    str = "0" + String(_count);
  }  else
  {
    str = String(_count);
  }
  return str;
}

 



#define STX_BBN_ENC_RESP	"$FF00"



void Encoder::setBbnCnt(unsigned int _cnt) 
{
	bbnCnt = _cnt;
	LogPrintln(" lg] bnEnc Cnt:" + String(bbnCnt));
}

void Encoder::setLmCnt(unsigned int _cnt) 
{
	lmCnt = _cnt;
	LogPrintln(" lg] LmEnc Cnt:" + String(lmCnt));
}

