/******************************************************
 SENSOR LIBRARY:
 
 Written by Enrico Formenti.  
 BSD license, check license.txt for more information
 All text above must be included in any redistribution.
 ******************************************************/

/* Sensor library
 
 DS18B20.CPP: methods for class DS18B20

written by Enrico Formenti
*/

/**
 \file DS18B20.cpp
 \brief Implementation of class DS18B20.
 \author Enrico Formenti
 \version 0.1
 \date 2012-2013
 \warning This software is provided "as is". The author is 
 not responsible for any damage of any kind caused by this
 software. Use it at your own risk.
 \copyright BSD license. See license.txt for more details. 
 All text above must be included in any redistribution. 
 */


#include "DS18B20.h"

void DS18B20::begin(void) {
  uint8_t i;
	
	_isAlarmOn = false;
	_isAlarmTriggered = false;
	_alarm_tmax = 0;
	_alarm_tmin = 0;
		
//	pinMode(_pin, INPUT);
//	bitmask = PIN_TO_BITMASK(_pin);
//	baseReg = PIN_TO_BASEREG(_pin);
#if ONEWIRE_SEARCH
	reset_search();
#endif
	
	if( !search(_adr, CMD_GENERIC_SEARCH) ) {
		setError(ERROR_NO_MORE_ADDRESSES);
		reset_search();
		return;
	}
	if( crc8(_adr, 7) != _adr[7] ) {
		setError(ERROR_INVALID_CRC);
		return;
	}
	
	_isParasitePower = getPowerSupplyMode();
	
	// present = 0neWire::reset();
	reset();
	select(_adr);
	write(CMD_READ_SCRATCHPAD);    // read scratchpad
	
	for(i=0;i<9;i++)
		_scratchpad[i] = OWcomponent::read();
	
	if(crc8(_scratchpad, 8) != _scratchpad[SCRATCHPAD_CRC]) {
		setError(ERROR_INVALID_CRC);
		_res = 12;  // set default increments and resolution
//		_resInc = .0625;
		return;
	}
		
	_res = 9 + (((_scratchpad[CONFIGURATION] & 0x60) << 1)>>6); // read # of resolution bits from configuration reg
//	_resInc = .0625 * (1<<(12-_res));  // increments	
}


float DS18B20::getTemperature(void) {
	uint8_t i;
	uint16_t tmp;
	
	reset();
	select(_adr);
//	OneWire::write(0x44,1);  // start temperature measurement and A/D conversion
	write(CMD_START_CONVERSION, _isParasitePower);    // start temperature measurement and A/D conversion
	if(_isParasitePower)  // just in case since after we have a delay instruction
		depower();
	
	// wait for conversion to finish
	delay(1000/(1<<(12-_res))); // conversion time is proportional to resolution
	                            // 750 should be enough for 12 bit resolution according to manual
	                            // here we add some extra delay to be absolutely sure...
	
	// present = OneWire::reset();
	reset();
	select(_adr);
	write(CMD_READ_SCRATCHPAD, _isParasitePower);    // read scratchpad
	
	for(i=0;i<9;i++)
		_scratchpad[i] = OWcomponent::read();
	
	if(crc8(_scratchpad, 8) != _scratchpad[SCRATCHPAD_CRC]) {
		setError(ERROR_INVALID_CRC);
		return 0;
	}
		
	tmp = (_scratchpad[TEMP_MSB]<<8)|_scratchpad[TEMP_LSB];
	tmp >>= (12-_res);
//	if (_data[1] & 0x80) 
//		tmp = (1+~tmp);
	
//	tmp = (tmp<<(16-_res))>>(16-_res);
	
//	if(_data[1] & 0x80) // if it is a negative number and it's a 2-complement number...
//	return (tmp>>4)+(tmp & 0xF)*_resInc;
	return (float)tmp/((1.0)*(1<<(_res-8)));
}

/**************
 setResolution: sets the number of bits of the resolution of measurement.
 
 retval: none
 input: r = number of resolution bits (must be between 9 and 12), otherwise an error
 (OUT_OF_RANGE) is raised and old resolution is maintained
 **************/
void DS18B20::setResolution(uint8_t r) {
	
	if(r<9 || r>12) {
		setError(ERROR_OUT_OF_RANGE);
		return;
	}
	_res = r;
	
	_datatmp[0]=_scratchpad[HIGH_ALARM_TEMP];
	_datatmp[1]=_scratchpad[LOW_ALARM_TEMP];
	_datatmp[2]=_scratchpad[CONFIGURATION];
	
	_datatmp[2] |= ((12-r)<<5); 
	
	reset();
	select(_addr);
	OWcomponent::write_bytes(_datatmp, 3, _isParasitePower);
	if(_isParasitePower)
		depower();
}

void DS18B20::setAlarm(int tmin, int tmax) {
	
	if(tmin<-55 || tmin>125 || tmax<-55 ||tmax>125 || tmin>tmax) {
		setError(ERROR_OUT_OF_RANGE);
		return;
	}
	
	_isAlarmOn = true;
		
	_alarm_tmin = tmin;
	_alarm_tmax = tmax;
	
	_datatmp[0]=tmin;
	_datatmp[1]=tmax;
	
	reset();
	select(_addr);
	write_bytes(_datatmp, 2, _isParasitePower);  
	if(_isParasitePower)
		depower();
}

boolean DS18B20::isAlarmTriggered(void) {
	
	uint8_t temp;
	
	if(!_isAlarmOn)
		return false;
	
	temp = _scratchpad[LOW_ALARM_TEMP]|(_scratchpad[HIGH_ALARM_TEMP]<<8);
	
	if( temp <= _alarm_tmin || temp >= _alarm_tmax)
		return true;

	return false;
}

boolean DS18B20::getPowerSupplyMode(void) {
	boolean retval = false;
	
	reset();
	select(_adr);
	write(CMD_READ_POWER_SUPPLY);
	
	if (read_bit() == 0) 
		retval = true;
	
	reset();
	
	return retval;
}
