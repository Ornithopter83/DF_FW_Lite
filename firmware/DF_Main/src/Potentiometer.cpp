 
 
 

#include "Common.h"
#include "Potentiometer.h"

Potentiometer::Potentiometer()
{

}

 


void Potentiometer::init()
{
  SPI.begin(TORQ_SCK, -1, TORQ_MOSI, TORQ_SS);   
  digitalPortWrite(TORQ_SS, TORQ_OFF_VALUE);     
}

 


void Potentiometer::digitalPortWrite(int _pinNum, int _val)
{
  digitalWrite(_pinNum, LOW);
  SPI.transfer(B00010001);  		 
  SPI.transfer(_val);       		 
  digitalWrite(_pinNum, HIGH);
}

 



void Potentiometer::setValue(String _val)
{
	 
	int val = _val.substring(3,(3+3)).toInt();

	outValue(val);
}

 



void Potentiometer::setValue(int val)
{
	outValue(val);
}

void Potentiometer::on(int val)
{
	outValue(val);
}

void Potentiometer::outValue(int val)
{

   
   

  if (val < RECV_MIN) { val = RECV_MIN; }
  if (val > RECV_MAX) { val = RECV_MAX; }

   
  if(TORQ_30W_LIMIT == torqWattAdj)
  {
  	if ( 35 < val)
  	{
  		 
  		 
  		val = (val*73/200) + 35;		   
  	}
  }

  


	if (val < RECV_MIN) { val = RECV_MIN; }
	if (val > RECV_MAX) { val = RECV_MAX; }
  
	digitalPortWrite(TORQ_SS, val);

		LogPrintln(" LG] TORQo " + String(val));
}

 
void Potentiometer:: respTorqOutVal(int val)
{
}

