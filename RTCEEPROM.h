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


#ifndef RTCEEPROM_H
#define RTCEEPROM_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Wire.h"
#include "I2Ccomponent.h"

#define RTC_STATUS 0xFF
#define ADDRESS 0x57
#define ERRCODE 0x8F
#define BUFFER 8
#define MAXMEM 0x7F

class RTCEEPROM : public I2Ccomponent{


	
	
public:	
	
	inline RTCEEPROM(void) :I2Ccomponent(ADDRESS) {  }
	/**
	\fn char getStatus(void)
	\brief gets the status of eeprom protection
	@returns a char indicating what part of memory is protected:
	\arg \c 0 means none
	\arg \c q means the upper quarter is protected
	\arg \c h means the upper half is protected
	\arg \c a means all of the eeprom is protected
	*/
	const char getStatus(void);
	/**
	\fn uint8_t writeSequentialBytes(const uint8_t addr, uint8_t *data,uint8_t length)
	\brief write a sequence of maximum 8 bytes onto the RTC eeprom using Wire
	@param addr is the memory address. It should be between 0x00 and 0x7F, otherwise the counter will overflow
	@param data is an array of bytes to write onto the memory
	@param length is the length of the data to write, in general different from the array length
	@returns a uint8_t indicating with the following values:
	\arg \c 0 means no errors
	\arg \c 0x7F means the operation has gone wrong
	*/
	uint8_t writeSequentialBytes(const uint8_t addr, uint8_t* data,uint8_t length);
	/**
	\fn uint8_t writeSingleByte(const uint8_t addr, uint8_t data)
	\brief write a single byte onto the RTC eeprom using Wire. It internally calls writeSequentialBytes
	@param addr is the memory address. It should be between 0x00 and 0x7F, otherwise the counter will overflow
	@param data is the data to write
	@returns a uint8_t indicating with the following values:
	\arg \c 0 means no errors
	\arg \c 0x7F means the operation has gone wrong
	@see writeSequentialBytes
	*/
	inline uint8_t writeSingleByte(const uint8_t addr, uint8_t data){return writeSequentialBytes(addr,&data,1);}
	/**
	\fn uint8_t readSequentialBytes(const uint8_t addr, uint8_t *data,uint8_t length)
	\brief reads a sequence of maximum 8 bytes from the RTC eeprom using Wire
	@param addr is the memory start address. It should be between 0x00 and 0x7F, otherwise the counter will overflow
	@param data is an array in which the data will be stored
	@param length is the length of the data to read
	@returns a uint8_t indicating with the following values:
	\arg \c 0 means no errors
	\arg \c 0x7F means the operation has gone wrong
	*/
	uint8_t readSequentialBytes(const uint8_t addr, uint8_t* data,uint8_t length);
	/**
	\fn uint8_t readSingleByte(const uint8_t addr, uint8_t data)
	\brief reads a single byte from the RTC eeprom using Wire and readSequentialBytes
	@param addr is the memory start address. It should be between 0x00 and 0x7F, otherwise the counter will overflow
	@param data is where the data will be stored
	@returns a uint8_t indicating with the following values:
	\arg \c 0 means no errors
	\arg \c 0x7F means the operation has gone wrong
	@see readSequentialBytes
	*/
	inline uint8_t readSingleByte(const uint8_t addr, uint8_t data){return readSequentialBytes(addr,&data,1);}
};

#endif
