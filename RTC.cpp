/******************************************************
 RTC LIBRARY:
 
 Written by Enrico Formenti.  
 Version 1.0 by Daniele Ratti
 BSD license, check license.txt for more information
 All text above must be included in any redistribution.
 ******************************************************/



/**
 \file RTC.cpp
 \brief Implementation of the RTC class.
 \author Enrico Formenti
 \author Daniele Ratti
 \version 1.0
 \date 2012-2013, 2015
 \warning This software is provided "as is". The author is 
 not responsible for any damage of any kind caused by this
 software. Use it at your own risk.
 \copyright BSD license. See license.txt for more details. 
 All text above must be included in any redistribution. 
*/
 
#include "RTC.h"

const uint8_t RTC::RTC_MAIN = 0x0;


void RTC::start(void) {
  writeByte(RTC::RTC_MAIN, readByte(RTC::RTC_MAIN) | 0x80);
}

void RTC::stop(void) {
	writeByte(RTC::RTC_MAIN, readByte(RTC::RTC_MAIN) & 0x7F);
}

void RTC::print(const uint8_t target, const uint8_t val){
    Serial.print("Value of ");
    Serial.print(target,HEX);
    Serial.println();
    Serial.print("is");
    Serial.print(val,HEX);
    Serial.println();
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
        tmp |= RTC_1224_FLAG;
    else
        tmp &= ~RTC_1224_FLAG;

			
	stop();
	writeByte(target+RTC_HOUR, tmp);
	start();
}

void RTC::setAlarmMatch(const uint8_t target, const char *format,...){
    uint8_t tmp;
    tmp=readByte(target+RTC_ALM_CFG)&0x8F; //resetting the current match
    switch (*format) {
        case 's':
        case 'S':
            writeByte(target+RTC_ALM_CFG,tmp);
            break;
        case 'm':
        case 'M':
            writeByte(target+RTC_ALM_CFG,tmp|0x10);
            break;
        case 'h':
        case 'H':
            writeByte(target+RTC_ALM_CFG,tmp|0x20);
            break;
        case 'd':
        case 'D':
            writeByte(target+RTC_ALM_CFG,tmp|0x30);
            break;
        case 'x':
        case 'X':
            writeByte(target+RTC_ALM_CFG,tmp|0x40);
            break;
        case 'a':
        case 'A':
            writeByte(target+RTC_ALM_CFG,tmp|0x70);
            break;
    }
}

const char RTC::getAlarmMatch(const uint8_t target) {
	char tmp;
    uint8_t val=readByte(target+RTC_ALM_CFG) & 0x70;
	switch(val>>4) {
		case 0:
            return 's';
			break;
		case 1: return 'm';
			break;
		case 2: return 'h';
			break;
		case 3: return 'd';
			break;
        case 4: return 'x';
            break;
		case 7:
                       return 'a';
			break;
	}
	return tmp;
}


void RTC::setAlarmLevel(const uint8_t target, const uint8_t lvl) {
	uint8_t tmp;
	
	tmp = readByte(target+RTC_ALM_CFG);
	
	if (lvl)
		tmp |= RTC_ALM_LVL_FLAG;
	else
        tmp &= ~RTC_ALM_LVL_FLAG;
		
	
	stop();
	writeByte(target+RTC_ALM_CFG, tmp);
	start();
}


boolean RTC::isLeapYear(void) { 
		
	return (boolean)((readByte(RTC_MAIN+RTC_MONTH) & 0x20)>>4);
}

void RTC::alarmFlagReset(const uint8_t target) {
	uint8_t a;
    uint8_t b;
	if (target == RTC_ALM0)
		a = RTC_ALM0_CONFIGURATION_BYTE;
	else
		a = RTC_ALM1_CONFIGURATION_BYTE;
    b=readByte(a);
    b&=~RTC_ALM_I_FLAG;
	stop();
	writeByte(a, b);
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
				if (val < 24) {
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
					writeByte(target+RTC_MINUTE, ((val/10)<<4)+(val%10));
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

void RTC::forceFeedback(const uint8_t target, const uint8_t val)
{
	writeByte(target,val);
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
				//if( val < 32 ) {
					stop();
					writeByte(target+RTC_YEAR, ((val/10)<<4)+(val%10));
					start();
					//}
				// else
					// setError(ERROR_OUT_OF_RANGE);
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
				*val = (readByte(target+RTC_DAY) & 0x7)%7;
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
                tmp=readByte(target+RTC_YEAR);
                *val = 10*(tmp>>4)+(tmp&0xF);
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
				if(!get1224Mode(target)) { // if 24H format
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
				tmp = readByte(target+RTC_MINUTE) & 0x7f;
				*val = 10*(tmp>>4)+(tmp&0xF);
				break;
			case 's' :
			case 'S' : // seconds
				val = (uint8_t *)va_arg(pl, int *);
				tmp = readByte(target+RTC_SECOND) & 0x7F;
				*val = (tmp>>4)*10+(tmp&0xF);
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
boolean RTC::isAlarmActive(const uint8_t target){
    boolean ret=false;
    uint8_t val=readByte(RTC_CONFIGURATION_BYTE);
    switch (target) {
        case RTC_ALM0:
            ret=(boolean)((val&0x10)>>4);
            break;
        case RTC_ALM1:
            ret=(boolean)((val&0x20)>>5);
        default:
            setError(ERROR_INVALID_FORMAT);
            break;
    }
    return ret;
}
const char RTC::getAlarmMode(void){
    uint8_t val = readByte(RTC_CONFIGURATION_BYTE);
    val&=0x30;
    switch (val>>4) {
        case 0:
            return 'n';
        case 1:
            return '0';
        case 2:
            return '1';
        case 3:
            return 'b';
        default:
            setError(ERROR_OUT_OF_RANGE);
    }
}
void RTC::configureAlarmMode(const char format){
    uint8_t val=readByte(RTC_CONFIGURATION_BYTE);
    val&=0xCF;//resets the conf bytes
    switch (format) {
        case '0':
            writeByte(RTC_CONFIGURATION_BYTE,(val|0x10));
            break;
        case '1':
            writeByte(RTC_CONFIGURATION_BYTE,val|0x20);
            break;
        case 'n':
            writeByte(RTC_CONFIGURATION_BYTE,val);
            break;
        case 'b':
            writeByte(RTC_CONFIGURATION_BYTE,val|0x30);
            break;
        default:
            setError(ERROR_INVALID_FORMAT);
            break;
    }
}
void RTC::getConfBits(){
    uint8_t val;
    //val = readByte(0x02);
   // print(0x02,val);
    val=readByte(0x07);
    print(0x07,val);
    val=readByte(0x0A);
    print(0x0A,val);
    val=readByte(0x0C);
    print(0x0C,val);
    val=readByte(0x0D);
    print(0x0D,val);
}
void RTC::printConfBit(const uint8_t reg){
    uint8_t val;
    val=readByte(reg);
    print(reg,val);
}

void RTC::setTrimming(uint8_t trimval){
stop();
writeByte(RTC_OSCTRIM,trimval);
start();	
}

void RTC::setSquareWaveOutput(uint8_t freqval){
	uint8_t val =readByte(RTC_CONFIGURATION_BYTE);
	val&=0xFC;
	val|=(1<<6);
	val|=(freqval);
	stop();
	writeByte(RTC_CONFIGURATION_BYTE,val);
	start();
}

// void setBit(uint8_t byte,uint8_t val){
// 	stop();
// 	writeByte(byte,val);
// 	start();
// }

void RTC::clearSquareWaveOutput(){
	uint8_t val=readByte(RTC_CONFIGURATION_BYTE);
	val&=0xBC;
	stop();
	writeByte(RTC_CONFIGURATION_BYTE,val);
	start();
}

