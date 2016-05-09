/**
 \file RTCMEMORY.cpp
 \brief Implementation of the RTCMEMORY class
 \author Daniele Ratti
 \version 1.0
 \date 2015-2016
 \warning This software is provided "as is". The author is 
 not responsible for any damage of any kind caused by this
 software. Use it at your own risk.
 \copyright BSD license. See license.txt for more details. 
 All text above must be included in any redistribution. 
*/


#include "RTCMEMORY.h"



void RTCMEMORY::writeEEpromBytes(const uint8_t addr, uint8_t *data,uint8_t length){
	uint8_t counter,err;
	if(length>BUFFER_EE)
		{
			setError(ERROR_OUT_OF_RANGE);
			return;
		}
	Wire.beginTransmission(ADDRESS_EE);
	Wire.write(addr);
	Wire.write(data,length);
	err=Wire.endTransmission();
	if(err!=0){
		setError(ERROR_WRITE_FAILURE);
		return;
	}
		
	for(counter=0;counter<100; counter++){
		_delay_us(300);
		Wire.beginTransmission(ADDRESS_EE);
		Wire.write(addr);
		err=Wire.endTransmission();
		if(err==0)
			break;
	}
	if(err!=0)
		setError(ERROR_WRITE_FAILURE);
}

void RTCMEMORY::writeEEpromBytesNoOF(const uint8_t addr, uint8_t *data,uint8_t length){
	uint8_t counter,err;
	uint8_t nextpage=8-(addr%8);
	if(length>BUFFER_EE||length>nextpage)
		{
			setError(ERROR_OUT_OF_RANGE);
			return;
		}
	Wire.beginTransmission(ADDRESS_EE);
	Wire.write(addr);
	Wire.write(data,length);
	err=Wire.endTransmission();
	if(err!=0){
		setError(ERROR_WRITE_FAILURE);
		return;
	}
		
	for(counter=0;counter<100; counter++){
		_delay_us(300);
		Wire.beginTransmission(ADDRESS_EE);
		Wire.write(addr);
		err=Wire.endTransmission();
		if(err==0)
			break;
	}
	if(err!=0)
		setError(ERROR_WRITE_FAILURE);
}

void RTCMEMORY::readEEpromBytes(const uint8_t addr, uint8_t *data,uint8_t length){
	uint8_t counter,err;
	if(length>BUFFER_EE){
		setError(ERROR_OUT_OF_RANGE);
		return;
	}
	Wire.beginTransmission(ADDRESS_EE);
	Wire.write(addr);
	err=Wire.endTransmission();
	if(err!=0){
			setError(ERROR_READ_FAILURE);
				return;
	}
	Wire.requestFrom(ADDRESS_EE,length);
	for(counter=0;counter<length;counter++){
		data[counter]=Wire.read();
	}
	
}

const char RTCMEMORY::getStatus(){
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
void RTCMEMORY::writeSRAMBytes(const uint8_t addr, uint8_t* data,uint8_t length){
		if(addr<RTC_SRAM_START||addr>RTC_SRAM_END)
			setError(ERROR_OUT_OF_RANGE);
		uint8_t err;
		Wire.beginTransmission(ADDRESS_SR);
		Wire.write(addr);
		Wire.write(data,length);
		err=Wire.endTransmission();
		if(err!=0)
			setError(ERROR_WRITE_FAILURE);
	}
void RTCMEMORY::readSRAMBytes(const uint8_t addr,uint8_t*data,uint8_t length){
		uint8_t err,counter;
		Wire.beginTransmission(ADDRESS_SR);
		Wire.write(addr);
		err=Wire.endTransmission();
		if(err!=0)
			setError(ERROR_READ_FAILURE);
		Wire.requestFrom(ADDRESS_SR,length);
		for(counter=0;counter<length;counter++){
			data[counter]=Wire.read();
	}
}
