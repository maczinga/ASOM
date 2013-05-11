/******************************************************
 RTC LIBRARY:
 
 Written by Enrico Formenti.  
 BSD license, check license.txt for more information
 All text above must be included in any redistribution.
 ******************************************************/



/**
 \file RTC.cpp
 \brief Implementation of the RTC class.
 \author Enrico Formenti
 \version 0.1
 \date 2012-2013
 \warning This software is provided "as is". The author is 
 not responsible for any damage of any kind caused by this
 software. Use it at your own risk.
 \copyright BSD license. See license.txt for more details. 
 All text above must be included in any redistribution. 
 */
 
#include "RTC.h"

const uint8_t RTC::RTC_MAIN = 0x0;
const uint8_t RTC::RTC_ALM0 = 0x0A;
const uint8_t RTC::RTC_ALM1 = 0x11;

void RTC::start(void) {
  writeByte(RTC::RTC_MAIN, readByte(RTC::RTC_MAIN) | 0x80);
}

void RTC::stop(void) {
	writeByte(RTC::RTC_MAIN, readByte(RTC::RTC_MAIN) & 0x7F);
}

void RTC::batterySupply(const boolean enable) {
	uint8_t tmp;
	
	tmp = readByte(RTC_MAIN+RTC_DAY);
	
	if( enable )
		tmp |= 0x08;
	else
		tmp &= 0xF7;

	stop();	
	writeByte(RTC_MAIN+RTC_DAY, tmp);
	start();
}

void RTC::set1224Mode(const uint8_t target, boolean mode) {
	uint8_t tmp;
	
	tmp = readByte(target+RTC_HOUR);
	
	if (mode)
		tmp &= ~RTC_1224_FLAG;
	else
		tmp |= RTC_1224_FLAG;
	
	stop();
	writeByte(target+RTC_HOUR, tmp);
	start();
}

uint8_t RTC::getAlarmMatch(const uint8_t target) {
	uint8_t tmp;
	
	switch(readByte(target+RTC_DAY) & 0x70) {
		case 0: tmp = 's';
			break;
		case 1: tmp = 'm';
			break;
		case 2: tmp = 'h';
			break;
		case 3: tmp = 'd';
			break;
		case 7: tmp = 'a';
			break;
	}
	return tmp;
}


void RTC::setAlarmLevel(const uint8_t target, const uint8_t lvl) {
	uint8_t tmp;
	
	tmp = readByte(target+RTC_DAY);
	
	if (lvl)
		tmp &= ~RTC_ALM_LVL_FLAG;
	else
		tmp |= RTC_ALM_LVL_FLAG;
	
	stop();
	writeByte(target+RTC_DAY, tmp);
	start();
}


boolean RTC::isLeapYear(void) { 
		
	return (boolean)((readByte(RTC_MAIN+RTC_MONTH) & 0x20)>>4);
}

void RTC::alarmFlagReset(const uint8_t target) {
	uint8_t a;
	
	if (target == RTC_ALM0)
		a = RTC_ALM0_CONFIGURATION_BYTE;
	else
		a = RTC_ALM1_CONFIGURATION_BYTE;
		
	stop();
	writeByte(a,readByte(a) & ~RTC_ALM_I_FLAG);
	start();
}

void RTC::setTime(const uint8_t target, const char *format, ...) {
	va_list pl;
	uint8_t val, tmp;
		
	va_start(pl, format);
	for(;*format != '\0';format++) {
		switch (*format) {
			case 'h' :
			case 'H' : // hours
				val = (uint8_t)va_arg(pl, int);
				if (val < 13) {
					tmp = readByte(target+RTC_HOUR) & 0xE0;
					stop();
					writeByte(target+RTC_HOUR, tmp|((val/10)<<4|(val%10)));
					start();
				}
				else
					setError(ERROR_OUT_OF_RANGE);
				break;
			case 'm' :
			case 'M' : // minutes
				val = (uint8_t)va_arg(pl, int);
				if(val<61) {
					tmp = readByte(target+RTC_MINUTE) & 0x80;
					stop();
					writeByte(target+RTC_MINUTE, tmp|((val/10)<<4|(val%10)));
					start();
				}
				else
					setError(ERROR_OUT_OF_RANGE);
				break;
			case 's' :
			case 'S' : // seconds
				val = (uint8_t)va_arg(pl, int);
				if (val < 61) {
					tmp = readByte(target+RTC_SECOND) & 0x80;
					stop();
					writeByte(target+RTC_SECOND, tmp|((val/10)<<4|(val%10)));
					start();
				}
				else
					setError(ERROR_OUT_OF_RANGE);
				break;
			default:
				setError(ERROR_INVALID_FORMAT);
			} /* end switch */
	}   
	va_end(pl);	
							  
	start();
}

void RTC::setDate(const uint8_t target, const char *format, ...) {
	va_list pl;
	uint8_t val, tmp;
		
	stop();
	
	va_start(pl, format);
	for(;*format != '\0';format++) {
		switch (*format) {
			case 'd' : // day
			case 'D' :
				val = (uint8_t)va_arg(pl, int);
				if( val < 8 ) {
					tmp = readByte(target+RTC_DAY) & 0xF8;
					stop();
					writeByte(target+RTC_DAY, tmp|val);
					start();
				}
				else
					setError(ERROR_OUT_OF_RANGE);
				break;
			case 'n' : // date
			case 'N' :
				val = (uint8_t)va_arg(pl, int);
				if( val < 32 ) {
					tmp = readByte(target+RTC_DATE) & 0xC0;
					stop();
					writeByte(target+RTC_DATE, tmp|((val/10)<<4)+(val%10));
					start();
				}
				else
					setError(ERROR_OUT_OF_RANGE);
				break;
			case 'm' : // month
			case 'M' :
				val = (uint8_t)va_arg(pl, int);
				if( val < 13 ) {
					tmp = readByte(target+RTC_DATE) & 0xE0;
					stop();
					writeByte(target+RTC_MONTH, tmp|((val/10)<<4)|(val%10));
					start();
				}
				else
					setError(ERROR_OUT_OF_RANGE);
				break;
			case 'y' : // year
				val = (uint8_t)va_arg(pl, int);
				if( val < 32 ) {
					stop();
					writeByte(target+RTC_YEAR, ((val/10)<<4)+(val%10));
					start();
				}
				else
					setError(ERROR_OUT_OF_RANGE);
				break;
			default:
				setError(ERROR_INVALID_FORMAT);
		} /* end switch */
	}   
	va_end(pl);	
}

void RTC::getDate(const uint8_t target, const char *format, ...) {
	va_list pl;
	uint8_t *val, tmp;
	
	va_start(pl, format);
	for(;*format != '\0';format++) {
		switch (*format) {
			case 'd' : // day
			case 'D' :
				val = (uint8_t*)va_arg(pl, int *);
				*val = readByte(target+RTC_DAY) & 0x7;				
				break;
			case 'n' : // date
			case 'N' :
				val = (uint8_t*)va_arg(pl, int *);
				tmp = readByte(target+RTC_DATE) & 0x3F;
				*val = 10*(tmp>>4)+(tmp&0xF);
				break;
			case 'm' : // month
			case 'M' :
				val = (uint8_t*)va_arg(pl, int *);
				tmp = readByte(target+RTC_MONTH) & 0x1F;
				*val = 10*(tmp>>4)+(tmp&0xF);
				break;
			case 'y' : // year
				val = (uint8_t *)va_arg(pl, int *);
				*val = readByte(target+RTC_YEAR);
				break;
			default:
				setError(ERROR_INVALID_FORMAT);
		} /* end switch */
	}   
	va_end(pl);
	
	start();
}

void RTC::getTime(const uint8_t target, const char *format, ...) {
	va_list pl;
	uint8_t *val, tmp;
		
	va_start(pl, format);
	for(;*format != '\0';format++) {
		switch (*format) {
			case 'a' :
			case 'A' : // AM/PM
				val = (uint8_t*)va_arg(pl, int *);
				*val = readByte(target+RTC_HOUR) & 0x20;
				break;
			case 'h' :
			case 'H' : // hours
				val = (uint8_t *)va_arg(pl, int *);
				tmp = readByte(target+RTC_HOUR);
				if(tmp & 0x40) { // if 24H format
					tmp &= 0x3F;
					*val = 10*(tmp>>4)+(tmp&0xf);
				}
				else {
					tmp &= 0x1F;
					*val = 10*(tmp>>4)+(tmp&0xF);
				}
				break;
			case 'm' :
			case 'M' : // minutes
				val = (uint8_t *)va_arg(pl, int *);
				tmp = readByte(target+RTC_MINUTE) & 0x70;
				*val = (tmp>>4)*10+(tmp&0xf);
				break;
			case 's' :
			case 'S' : // seconds
				val = (uint8_t *)va_arg(pl, int *);
				tmp = readByte(target+RTC_SECOND) & 0x7F;
				*val = (tmp>>4)*10+(tmp&0xf);
				break;
			case 't' :
			case 'T' : // 12/24 
				val = (uint8_t *)va_arg(pl, int *);
				*val = readByte(target+RTC_HOUR) & 0x40;
				break;
			default:
				setError(ERROR_INVALID_FORMAT);
		} /* end switch */
	}   
	va_end(pl);	
}
