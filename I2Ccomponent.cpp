/******************************************************
 RTC LIBRARY:
 
 Written by Enrico Formenti.  
 BSD license, check license.txt for more information
 All text above must be included in any redistribution.
 ******************************************************/



/**
 \file I2Ccomponent.cpp
 \brief Implementation of the I2Ccomponent class.
 \author Enrico Formenti
 \version 0.1
 \date 2012-2013
 \warning This software is provided "as is". The author is 
 not responsible for any damage of any kind caused by this
 software. Use it at your own risk.
 \copyright BSD license. See license.txt for more details. 
 All text above must be included in any redistribution. 
 */
 
#include "I2Ccomponent.h"


void I2Ccomponent::writeByte(const uint8_t adr, const uint8_t data) {
  
	Wire.beginTransmission(_address);
	Wire.write(adr);
	Wire.write(data);
	Wire.endTransmission();	

}

uint8_t I2Ccomponent::readByte(const uint8_t adr) {
	uint8_t data;
	
	Wire.beginTransmission(_address);
	Wire.write(adr);
	Wire.endTransmission();
	
	Wire.requestFrom((uint8_t)_address,(uint8_t)1);
	while (Wire.available()) data=Wire.read();
	
	return data;
}
