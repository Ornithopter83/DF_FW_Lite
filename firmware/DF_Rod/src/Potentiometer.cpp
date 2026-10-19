 
 
 

#include "Common.h"
#include "Potentiometer.h"

Potentiometer::Potentiometer()
{

}

void Potentiometer::init()
{
	 
  SPI.begin(SPI_SCK, -1, SPI_MOSI, SPI_SS);
  pinMode (SPI_SS, OUTPUT);
  digitalPortWrite(SPI_SS, DEFAULT_VALUE);
   
  
	 
}

 


void Potentiometer::digitalPortWrite(int _pinNum, int _val)
{
  digitalWrite(_pinNum, LOW);
  SPI.transfer(B00010001);
  SPI.transfer(_val);
  digitalWrite(_pinNum, HIGH);
}

 


void Potentiometer::setValue(String _str)
{
	 
	int _val = _str.substring(0).toInt();		 

	outValue(_val);
}

void Potentiometer::setValue(int _val)
{
	outValue(_val);
}

 


void Potentiometer::outValue(int _val)
{
  if (_val < RECV_MIN)
  {
    _val = RECV_MIN;
  }

  if (_val > RECV_MAX)
  {
    _val = RECV_MAX;
  }

	 
  
   
  _val = 255 - _val;
  digitalPortWrite(SPI_SS, _val);
}

