/**
 \file RTCMEMORY.h
 \brief Definition of a class for dealing with MCP7921X EEPROM and SRAM.
 \details Header file containing the definition of the RTCMEMORY class.
 \author Daniele Ratti
 \version 1.0
 \date 2015-2016
 \warning This software is provided "as is". The author is 
 not responsible for any damage of any kind caused by this
 software. Use it at your own risk.
 \copyright BSD license. See license.txt for more details. 
 All text above must be included in any redistribution. 
*/


#ifndef RTCMEMORY_H
#define RTCMEMORY_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Wire.h"
#include "I2Ccomponent.h"
#include "Error.h"
//@{
	
/**
\def RTC_STATUS 0xFF
\brief the status pointer address
*/
#define RTC_STATUS 0xFF
/**
\def ADDRESS_EE 0x57
\brief the I2C address for the EEPROM
*/
#define ADDRESS_EE 0x57
/**
\def ADDRESS_SR 0x6F
\brief the I2C address for the SRAM
*/
#define ADDRESS_SR 0x6F
/**
\def BUFFER_EE 8
\brief the buffer size for the EEPROM
\warning this should not be called BUFFER, since otherwise it would overwrite the Wire buffer, which is used by SRAM.
*/
#define BUFFER_EE 8
//@}

class RTCMEMORY : public I2Ccomponent, public Error{


private:
	/**
	\var RTC_SRAM_START
	\brief the starting point of the SRAM
	\warning use the variable for future compatibility issues
	*/
		static const uint8_t RTC_SRAM_START=0x20;
		/**
		\var RTC_SRAM_END
		\brief the ending point of the SRAM
		\warning use the variable for future compatibility issues
		*/
		static const uint8_t RTC_SRAM_END=0x5F;
		/**
		\var MAXMEM
		\brief the max EEPROM address
		\warning use the variable for future compatibility issues
		*/
		static const uint8_t MAXMEM=0x7F;	
	
public:	
	

	
	inline RTCMEMORY(void): I2Ccomponent(ADDRESS_EE){}
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
	\fn void writeEEpromBytes(const uint8_t addr, uint8_t *data,uint8_t length)
	\brief write a sequence of maximum 8 bytes onto the RTC eeprom using Wire
	@param addr is the memory address. It should be between 0x00 and 0x7F, otherwise the counter will overflow
	@param data is an array of bytes to write onto the memory
	@param length is the length of the data to write, in general different from the array length
	\warning the memory is paged by 8 bytes.
	 Every write operation exceeding the page length will result in overflow, 
	 overwriting effectively the previous bytes of the page. No overflow checks are performed.
	*/
	void writeEEpromBytes(const uint8_t addr, uint8_t* data,uint8_t length);
	/**
	\fn writeEEpromBytesNoOF(const uint8_t addr, uint8_t* data,uint8_t length)
	\brief write a sequence of maximum 8 bytes onto the RTC eeprom using Wire.
	The method only writes if the  length is less than or equal to the number of bytes from the start address and the end of page. 
	@param addr is the memory address. It should be between 0x00 and 0x7F, otherwise the counter will overflow
	@param data is an array of bytes to write onto the memory
	@param length is the length of the data to write, in general different from the array length
	\remark the memory is paged by 8 bytes.  
	*/
	void writeEEpromBytesNoOF(const uint8_t addr, uint8_t* data,uint8_t length);
	/**
	\fn void readEEpromBytes(const uint8_t addr, uint8_t *data,uint8_t length)
	\brief reads a sequence of maximum 8 bytes from the RTC eeprom using Wire
	@param addr is the memory start address. It should be between 0x00 and 0x7F, otherwise the counter will overflow
	@param data is an array in which the data will be stored
	@param length is the length of the data to read
	*/
	void readEEpromBytes(const uint8_t addr, uint8_t* data,uint8_t length);
	/**
	\fn void writeSRAMBytes(const uint8_t addr, uint8_t* data,uint8_t length)
	\brief writes on the SRAM
	@param addr the starting address, which must be between \c RTC_SRAM_START and \c RTC_SRAM_END
	@param data the array to write
	@param length the number of bytes to write
	*/
	void writeSRAMBytes(const uint8_t addr, uint8_t* data,uint8_t length);
	/**
	\fn readSRAMBytes(const uint8_t addr,uint8_t*data,uint8_t length)
	\brief reads bytes from SRAM
	@param addr is the starting address, which must be included between \c RTC_SRAM_START and \c RTC_SRAM_END
	@param data is the array on which the data is stored
	@param length is the number of bytes to read
	*/
	void readSRAMBytes(const uint8_t addr,uint8_t*data,uint8_t length);
};

#endif
