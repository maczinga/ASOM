/**
 \file RTCEEPROM.h
 \brief Definition of a class for dealing with MCP7921X EEPROM and status.
 \details Header file containing the definition of the RTCEEPROM class.
 \author Daniele Ratti
 \version 0.1
 \date 2015-2016
 \warning This software is provided "as is". The author is 
 not responsible for any damage of any kind caused by this
 software. Use it at your own risk.
 \copyright BSD license. See license.txt for more details. 
 All text above must be included in any redistribution. 
*/


#include "RTCEEPROM.h"



uint8_t RTCEEPROM::writeSequentialBytes(const uint8_t addr, uint8_t *data,uint8_t length){
	uint8_t err=ERRCODE;
	uint8_t counter;
	if(length>BUFFER)
		return ERRCODE;
	Wire.beginTransmission(ADDRESS);
	Wire.write(addr);
	Wire.write(data,length);
	err=Wire.endTransmission();
	if(err!=0)
		return ERRCODE;
	for(counter=0;counter<100; counter++){
		_delay_us(300);
		Wire.beginTransmission(ADDRESS);
		Wire.write(addr);
		err=Wire.endTransmission();
		if(err==0)
			break;
	}
	return err==0?err:ERRCODE;
}

uint8_t RTCEEPROM::readSequentialBytes(const uint8_t addr, uint8_t *data,uint8_t length){
	uint8_t err=ERRCODE;
	uint8_t counter;
	if(length>BUFFER)
		return ERRCODE;
	Wire.beginTransmission(ADDRESS);
	Wire.write(addr);
	err=Wire.endTransmission();
	if(err!=0)
		return ERRCODE;
	Wire.requestFrom(ADDRESS,length);
	for(counter=0;counter<length;counter++){
		data[counter]=Wire.read();
	}
	return 0;
}

const char RTCEEPROM::getStatus(){
	uint8_t val=readByte(RTC_STATUS);
	switch(val){
		case 0:
		 return '0';
		case 1:
		return 'q';
		case 2:
		return 'h';
		case 3:
		return 'a';
		default:
		 return '\0';
	}
	
}
