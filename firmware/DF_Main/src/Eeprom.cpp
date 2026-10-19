 
 
 

#include "Common.h"
#include "Eeprom.h"


 
#define CONTWRITE 0xA0
#define CONTREAD  0xA1

 
#define I2C_SLAVE_ADDR	0x50


Eeprom::Eeprom()
{

}

void Eeprom::init()
{
	bool rts;

	rts = Wire.begin();		 

	 

}


int Eeprom::cReadSend(unsigned int len)
{
	return (int)Wire.requestFrom(I2C_SLAVE_ADDR, len);
	 
}


 

int Eeprom::readSend(unsigned int reqOffset, unsigned int reqLen)
{
	
	unsigned int max = (reqOffset + reqLen);
	if( !reqLen || (0x8000 < reqLen) || (0x7FFF < reqOffset))
	{
		return (-1);	 
	}

	else
	{
		Wire.beginTransmission(I2C_SLAVE_ADDR);
		Wire.write( (byte)(reqOffset & 0x7F00 >> 8) );	 
		Wire.write( (byte)(reqOffset & 0x00FF) );  
		 
		int rts = Wire.endTransmission(false);				 

		Wire.requestFrom(I2C_SLAVE_ADDR, reqLen);
		
		return (rts);	 
	}

}

int Eeprom::read(byte* buffPtr, unsigned int reqOffset, unsigned int reqLen)
{
	int rdLen = Wire.available();
	if(rdLen)
	{
		unsigned int i;
		for(i=reqOffset; i< rdLen; i++)
		{
			buffPtr[i]= Wire.read();
		}
	}
	 
	return rdLen;
}

 
int Eeprom::buffSave(unsigned int reqOffset, unsigned int reqLen)
{
	
	unsigned int max = (reqOffset+reqLen);
	if( !reqLen || (0x8000 < reqLen) || (0x7FFF < reqOffset))
	{
		return (-1);	 
	}

	else
	{
		unsigned int i,j;
		
		if(0x7FFF < max)
		{
			for(j=reqOffset; j<0x8000; j++)
			{
				buff[j] = Wire.read();
			}
 			max -= 0x8000;
			for(i=0; i<max; i++)
			{
				buff[i] = Wire.read();
			}
			i= i+(j-reqOffset+1);
		}
		else
		{
 			for(i=reqOffset; i<max; i++)
			{
				buff[i] = Wire.read();
			}
			i=(i-reqOffset+1);
		}

		return (i);	 
	}

}

 
 
 
void Eeprom::grazingGeddong(byte* buffPtr, unsigned int reqAddr, unsigned int reqLen)
{

	unsigned int max = (reqAddr+reqLen);
	String buffStr="";
	int i = 0;

	 
	for(i=reqAddr; i<max; i++)
	{
		buffStr += nib2char(buffPtr[i]);
	}

}

String Eeprom::nib2char(byte _nib)
{
	String str="";
	char nibChar;
	byte nib2;
	
	nib2 = (_nib & 0xF0) >> 4;
	if(10 > nib2 ) { nibChar = nib2 + '0'; }
	else		   { nibChar = nib2 - 10 + 'A'; }
	str += nibChar;
	
	nib2 = (_nib & 0x0F);
	if(10 > nib2 ) { nibChar = nib2 + '0'; }
	else		   { nibChar = nib2 - 10 + 'A'; }
	str += nibChar;

	str += ",";

	return str;	
}
 


int Eeprom::writeSend(unsigned int reqOffset, unsigned int reqLen)
{

}

int Eeprom::write(byte *buffPtr, unsigned int reqOffset, unsigned int reqLen)
{
	
	unsigned int max;
	max = (reqOffset + reqLen);
	if( !reqLen || (0x7FFF < reqOffset) || (0x8000 < max) )
	{
		return (-1);	 
	}
	else
	{
		Wire.beginTransmission(I2C_SLAVE_ADDR);
		Wire.write( (byte)(reqOffset & 0x7F00 >> 8) );
		Wire.write( (byte)(reqOffset & 0x00FF) );
		unsigned int i;
		for(i=reqOffset; i<max; i++)
		{
			Wire.write(buffPtr[i]);
		}
		return (int)Wire.endTransmission();
	}
}


 








 



void Eeprom::checksum()
{


 
}

 


























