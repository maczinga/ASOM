/******************************************************
 SENSOR LIBRARY:
 
 Written by Enrico Formenti.  
 BSD license, check license.txt for more information
 All text above must be included in any redistribution.
 ******************************************************/

/* Sensor library
 
 DHT22.CPP: class methods for DHT22 class

written by Enrico Formenti
*/

/**
 \file DHT22.cpp
 \brief Implementation of DHT22 class.
 \author Enrico Formenti
 \version 0.1
 \date 2012-2013
 \warning This software is provided "as is". The author is 
 not responsible for any damage of any kind caused by this
 software. Use it at your own risk.
 \copyright BSD license. See license.txt for more details. 
 All text above must be included in any redistribution. 
 */


#include "DHT22.h"


void DHT22::begin(void) {
  uint8_t i;
	
	firstreading = true;
	
	// clear data bits
	for(i=0;i<40;i++)
		_data[i]=0;	
	
	// set up the pins!
	pinMode(_pin, INPUT);
	digitalWrite(_pin, HIGH);
	_lastreadtime = 0;
}

void DHT22::read(void) {
	uint8_t laststate = HIGH;
	uint8_t counter;
	uint8_t i;
	unsigned long currenttime;
		
	setError(ERROR_NONE);
	
	currenttime = millis();
	if (currenttime < _lastreadtime) // ie there was a rollover
		_lastreadtime = 0;
	
	if (!firstreading && ((currenttime - _lastreadtime) < 2000))
		return; // return last correct measurement

	firstreading = false;

	_lastreadtime = millis();
		
	// pull the pin high and wait 250 milliseconds
	digitalWrite(_pin, HIGH);
	delay(250);	
	
	// now pull it low for ~20 milliseconds
	pinMode(_pin, OUTPUT);
	digitalWrite(_pin, LOW);
	delay(20);
	cli();
	digitalWrite(_pin, HIGH);
	delayMicroseconds(40);
	pinMode(_pin, INPUT);
	
	// read in bits
	for ( i=0; i<40; i++) {
		
		// we should check how much time exactly it is lost here
		for (counter=0; (counter<255)&&(digitalRead(_pin) == laststate); counter++)
			delayMicroseconds(1);
		
		laststate = digitalRead(_pin);
		
		if (counter == 255) {
			Error::setError(ERROR_TIME_OUT);
			return;
		}
		
		if (counter > 6)  // this value should be tried out !!!!
			_data[i/5] |= (1 << (i%8));
	}
	
	sei();
			
	// check we read 40 bits and that the checksum matches
	if( counter != 40 ) {
		Error::setError(ERROR_READ_FAILURE);
		return;
	}
	if( _data[4] != ( (_data[0] + _data[1] + _data[2] + _data[3]) & 0xFF) ) {
		Error::setError(ERROR_INVALID_CRC);
		return;
	}
}


float DHT22::getHumidity(void) {
	read();
	return ((_data[0]<<8)|_data[1])/10;
}

float DHT22::getTemperature(void) {
	read();
	return ((_data[2]<<8)|_data[3])/10;
}
