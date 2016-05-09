/**
 \mainpage RTC Library
 \par
 A class for playing with a RTC module for Arduino, based on the chip MCP79410.
 */
/**
 \file RTC.h
 \brief Definition of the RTC class.
 \details Header file containing the definition of the RTC class.
 \author Enrico Formenti
\author Daniele Ratti
 \version 1.5
 \date 2012-2016
 \warning This software is provided "as is". The author is 
 not responsible for any damage of any kind caused by this
 software. Use it at your own risk.
 \copyright BSD license. See license.txt for more details. 
 All text above must be included in any redistribution. 
*/

#ifndef RTC_H
#define RTC_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "RTCMEMORY.h"
#include "I2Ccomponent.h"
#include "Component.h"
#include "Error.h"

//@{

/**
\def RTC_SECOND 0x0
\brief the byte address for the seconds register, in each page (main, alm0, alm1)
*/
#define RTC_SECOND 0x0
/**
\def RTC_MINUTE 0x01
\brief the byte address for the minutes register, in each page (main, alm0, alm1)
*/
#define RTC_MINUTE 0x01
/**
\def RTC_HOUR 0x02
\brief the byte address for the hours register, in each page (main, alm0, alm1)
*/
#define RTC_HOUR 0x02
/**
\def RTC_DAY 0x03
\brief the byte address for the weekday register, in each page (main, alm0, alm1)
*/
#define RTC_DAY 0x03
/**
\def RTC_DATE 0x04
\brief the byte address for the date (day of month) register, in each page (main, alm0, alm1)
*/
#define RTC_DATE 0x04
/**
\def RTC_MONTH 0x05
\brief the byte address for the month register, in each page (main, alm0, alm1)	
*/
#define RTC_MONTH 0x05
/**
\def RTC_YEAR 0x06
\brief the byte address for the seconds register, in each page (main, alm0, alm1)
\remark this byte is only meaningful for the main clock register, since the alarms make no use of the year. 
*/
#define RTC_YEAR 0x06
/**
\def RTC_OSCTRIM 0x08
\brief	the byte address for the oscillator trimming, used to calibrate the RTCC
*/
#define RTC_OSCTRIM 0x08

/**
\def RTC_ALM_I_FLAG 0x08
\brief used to reset the alarm.
*/
#define RTC_ALM_I_FLAG 0x08
/**
\def RTC_1224_FLAG 0x40
\brief	the byte address for the 12/24 hour mode flag
*/
#define RTC_1224_FLAG 0x40
/**
\def RTC_ALM_LVL_FLAG 0x80
\brief	the byte address to set whether the multifunction pin is high or low when an alarm is triggered.
*/
#define RTC_ALM_LVL_FLAG 0x80

/**
\def RTC_ALM_CFG 0x03
\brief	the byte address for the alarm configuration register in the alarms page
*/
#define RTC_ALM_CFG 0x03
/**
\def RTC_CONFIGURATION_BYTE 0x07
\brief	the byte address for the RTC configuration register
*/
#define RTC_CONFIGURATION_BYTE 0x07

/**
\def RTC_ALM0_CONFIGURATION_BYTE 0x0D
\brief	the address to reset the ALM0 when is triggered
*/
#define RTC_ALM0_CONFIGURATION_BYTE 0x0D
/**
\def RTC_ALM1_CONFIGURATION_BYTE 0x14
\brief	the address to reset the ALM1 when is triggered
*/
#define RTC_ALM1_CONFIGURATION_BYTE 0x14

//@}

/**
 \class RTC RTC.h
 \brief A class for real time clock module based on MCP79410 chip.
 
 This class provides an interface for the component, defining a driver for almost all of the
 functionalities described in the datasheet. The Error class is used to set errors on the
 error buffer, and the I2Ccomponent class contains the basic I2C communication specifications.
 
 A specific class has been created to manage the memory (EEPROM and SRAM) communications,
 and is incapsulated as a strategy class.
 */

class RTC : public I2Ccomponent, public Component, public Error {
private:

	/**
	 \fn void start(void)
	 \brief Starts the main clock
	*/ 
	void start(void);
	/**
	 \fn void stop(void)
	 \brief Stops the main clock
	 */ 
	void stop(void);
	/**
     \fn void print(void)
     \brief prints on serial
     @param target is the bit printed
     @param value represent the value of the bit to print
     */
    void print(const uint8_t target,const uint8_t val);
	
	/** 
	 \fn uint8_t readSingleByteFromMemory(const uint8_t address,boolean isEEprom)
	 \brief reads a single byte from a given memory address
	 @param address represents the byte address
	 @param isEEprom a boolean, \c true when reading from EEProm, \c false when reading from SRAM
	 \return the byte value stored in the memory
	 \remark the method uses readBytesFromMemory
	 @see readBytesFromMemory
	*/
	uint8_t readSingleByteFromMemory(const uint8_t address,boolean isEEprom);
	/** 
	 \fn void readBytesFromMemory(const uint8_t address,uint8_t*data,uint8_t length,boolean isEEprom)
	 \brief reads a set of bytes from the memory
	 @param address represents the starting memory address
	 @param data is a pointer to an array (single or multiple valued) of bytes
	 @param length is the number of bytes to read from the start
	 @param isEEprom is a boolean assuming value \c True when reading from EEPROM, \c False when reading from SRAM
	*/
	void readBytesFromMemory(const uint8_t address,uint8_t*data,uint8_t length,boolean isEEprom);
	/**
	 \fn void writeBytesToMemory(const uint8_t address,uint8_t*data,uint8_t length,boolean isEEprom)
	 \brief writes a set of bytes to memory, starting from a specific address
	 @param address represents the starting memory address
	 @param data is the pointer to the array or value to write
	 @param length is the number of bytes to write from the start
	 @param isEEprom is a boolean assuming value \c True when writing to EEPROM, \c False when writing to SRAM
	*/
	void writeBytesToMemory(const uint8_t address,uint8_t*data,uint8_t length,boolean isEEprom);
	
		/**
		\var RTCMEMORY _mem
		\brief the memory strategy object
		*/
	RTCMEMORY _mem;
	
	/**
	\var boolean _allowEEpromOverflow
	\brief considers whether the EEPROM pointer is allowed to overflow when more than a page is written
	*/
	boolean _allowEEpromOverflow;
    
public:
	/**
	 \var static const uint8_t RTC_MAIN
	 \brief Address of the main clock.
	 \warning Use the variable in your programs. Direct use of the value \c 0x0 might result incompatible with future versions.
	 */
	static const uint8_t RTC_MAIN;   
	/**
	 \var static const uint8_t RTC_ALM0
	 \brief Address of alarm 0.
	 \warning Use the variable in your programs. Direct use of the value \c 0x0A might result incompatible with future versions.
	 */
	static const uint8_t RTC_ALM0=0x0A;
	/**
	 \var static const uint8_t RTC_ALM1
	 \brief Address of alarm 1.
	 \warning Use the variable in your programs. Direct use of the value \c 0x11 might result incompatible with future versions.
	 */
	static const uint8_t RTC_ALM1=0x11;
	
	/**
	 \fn RTC(void) : I2Ccomponent(0x6F)
	 \brief Default Constructor
	*/
	inline RTC(void) : I2Ccomponent(0x6F), Component(1,1) {  _mem=RTCMEMORY(); _allowEEpromOverflow=true;}
	/**
	\fn RTC(boolean allowOverflow):I2Ccomponent(0x6F)
	\brief extendend constructor for specifically allow or disallow the EEprom page overflow
	@param allowOverflow boolean \c True when the page overflow is allowed
	*/
	inline RTC(boolean allowOverflow):I2Ccomponent(0x6F),Component(1,1){_allowEEpromOverflow=allowOverflow; _mem=RTCMEMORY();}
	/**
	  \fn void setDate(const uint8_t target, const char *format, ...)
	  \brief Sets the date for the main clock or for one of the alarms.
	  @param target can take one of the three vales: RTC_MAIN, RTC_ALM0, RTC_ALM1 for main clock, alarm 0 and alarm 1, respectively.
	  @param format chain of characters indicating the variables to set, similarly to classical printf function of C language. Here
	         are the possibles characters:
			\arg \c d : number indicating the day of the week, 1 = monday, 2 = tuedsay, etc.
			\arg \c D : same as \c d
			\arg \c n : number of the day (ranging from 1 to 31)
			\arg \c N : same as \c n
			\arg \c m : number indicating the month, 1 = january, 2 = febrary, etc.
			\arg \c M : same as \c m
			\arg \c y : year (ranging from 0 to 99)
			\arg \c Y : same as \c y
	  \n
	  \remark Parameters are processed according to the order of appearence in \c format.
	  @see getDate, setTime, getTime
	*/
	void setDate(const uint8_t target, const char *format, ...);
	/**
	 \fn void getDate(const uint8_t target, const char *format, ...)
	 \brief Read the date from the main clock or from one of the alarms.
	 @param target can take one of the three vales: RTC_MAIN, RTC_ALM0, RTC_ALM1 for main clock, alarm 0 and alarm 1, respectively.
	 @param format chain of characters indicating the variables to set, similarly to classical printf function of C language. Here
	 are the possibles characters:
	 \arg \c d : number indicating the day of the week, 1 = monday, 2 = tuedsay, etc.
	 \arg \c D : same as \c d
	 \arg \c n : number of the day (ranging from 1 to 31)
	 \arg \c N : same as \c n
	 \arg \c m : number indicating the month, 1 = january, 2 = febrary, etc.
	 \arg \c M : same as \c m
	 \arg \c y : year (ranging from 0 to 99)
	 \arg \c Y : same as \c y
	 \n
	 \remark Parameters are processed according to the order of appearence in \c format.
	 @see setDate, setTime, getTime
	 */
	void getDate(const uint8_t target, const char *format, ...);
	/**
	 \fn void setTime(const uint8_t target, const char *format, ...)
	 \brief Sets the time for the onboard clock or for one of the alarms.
	 @param target can take one of the three vales: RTC_MAIN, RTC_ALM0, RTC_ALM1 for main clock, alarm 0 and alarm 1, respectively.
	 @param format chain of characters indicating the variables to set, similarly to classical printf function of C language. Here
	 are the possibles characters:
	 \arg \c s : seconds (ranging from 0 to 59)
	 \arg \c S : same as \c s
	 \arg \c m : minutes (ranging from 0 to 59)
	 \arg \c M : same as \c m
	 \arg \c h : hour (ranging from 0 to 24 or from 0 to 12 according to the 12/24 display format)
	 \arg \c H : same as \c h
	 \n
	 \remark Parameters are processed according to the order of appearence in \c format.
	 @see set1224Mode, getTime, setDate, getDate
	 */
	void setTime(const uint8_t target, const char *format, ...);
	
	/**
	 \fn void getTime(const uint8_t target, const char *format, ...)
	 \brief Reads the time from the main clock or from one of the alarms.
	 @param target can take one of the three vales: RTC_MAIN, RTC_ALM0, RTC_ALM1 for main clock, alarm 0 and alarm 1, respectively.
	 @param format chain of characters indicating the variables to set, similarly to classical printf function of C language. Here
	 are the possibles characters:
	 \arg \c s : seconds (ranging from 0 to 59)
	 \arg \c S : same as \c s
	 \arg \c m : minutes (ranging from 0 to 59)
	 \arg \c M : same as \c m
	 \arg \c h : hour (ranging from 0 to 24 or from 0 to 12 according to the 12/24 display format)
	 \arg \c H : same as \c h
	 \n
	 \remark Parameters are processed according to the order of appearence in \c format.
	 @see set1224Mode, getTime, setTime, getDate, setDate
	 */
	void getTime(const uint8_t target, const char *format, ...);
	/**
	 \fn boolean isAlarmTriggered(const uint8_t target)
	 \brief Returns true if the alarm for the given target has been triggered.
	 @param target can take one of the two vales: \c RTC_ALM0, \c RTC_ALM1 for alarm 0 and alarm 1, respectively.
	 \remark The flag 'alarm triggered' will stay on untill reset from software. Use the function \c alarmFlagReset to reset it
	 and start waiting for another alarm event.
	 @see alarmFlagReset, setTime, setDate, setAlarmMatch
	*/
	inline boolean isAlarmTriggered(const uint8_t target) { return (boolean)((readByte(target+RTC_DAY) & 0x08)>>3); }
	/**
	 \fn void alarmFlagReset(const uint8_t target)
	 \brief Resets the alarm flag for the \c target alarm.
	 @param target can take one of the two vales: \c RTC_ALM0, \c RTC_ALM1 for alarm 0 and alarm 1, respectively.
	 \remark The flag 'alarm triggered' will stay on untill reset from software. Use the function \c alarmFlagReset to reset it
	 and start waiting for another alarm event.
	 @see isAlarmTriggered, setAlarmMatch
	 */	
	void alarmFlagReset(const uint8_t target);
	/**
	 \fn void setAlarmMatch(const uint8_t target, const char format)
	 \brief Sets the criteria used by the module for trigering the alarm \c target.
	 @param target can take one of the two vales: \c RTC_ALM0, \c RTC_ALM1 for alarm 0 and alarm 1, respectively.
	 @param format character indicating the match criteria. Here are the admissible values:
	 \arg \c s : seconds
	 \arg \c S : same as \c s
	 \arg \c m : minutes
	 \arg \c M : same as \c m
	 \arg \c h : hours
	 \arg \c H : same as \c h
	 \arg \c d : day (alarm triggered at 12:00:00 AM)
	 \arg \c D : same as \c d
     \arg \c x : date
     \arg \c X : same as x
	 \arg \c a : matches seconds, minutes, hours, day, date, month.
	 \arg \c A : same as \c a
	 \n
     \remark Parameters are processed according to the order of appearence in \c format.	 
	 @see isAlarmTriggered, alarmFlagReset
	*/
	void setAlarmMatch(const uint8_t target, const char *format,...);
	/**
	 \fn void getAlarmMatch(const uint8_t target)
	 \brief Gets the criteria used by the module for triggering the alarm \c target.
	 @param target can take one of the two vales: \c RTC_ALM0, \c RTC_ALM1 for alarm 0 and alarm 1, respectively.
	 @return a character indicating the match criteria. Here are the possible values:
	 \arg \c s : seconds
	 \arg \c m : minutes
	 \arg \c h : hours
	 \arg \c d : day (alarm triggered at 12:00:00 AM)
     \arg \c x : date
	 \arg \c a : matches seconds, minutes, hours, day, date, month.
	 \n
	 @see setAlarmMatrch, isAlarmTriggered, alarmFlagReset
	 */	
	const char getAlarmMatch(const uint8_t target);
	/**
	 \fn boolean get1224Mode(const uint8_t target)
	 \brief Returns the display mode for the target clock or alarm.
	 @param target can take one of the three vales: \c RTC_MAIN, \c RTC_ALM0, \c RTC_ALM1 for main clock, alarm 0 and alarm 1, respectively.
	 \return \c True if the target clock/alarm is in 12 hours display mode, \c False for 24 hours mode.
	 @see set1224Mode
	 */ 
	inline boolean get1224Mode(const uint8_t target) { return (boolean)(readByte(target+RTC_HOUR) & 0x40)>>6; }
	/**
	 \fn void set1224Mode(const uint8_t target, const boolean mode)
	 \brief Sets the clock display mode for the given target clock or alarm.
	 @param target can take one of the three vales: \c RTC_MAIN, \c RTC_ALM0, \c RTC_ALM1 for main clock, alarm 0 and alarm 1, respectively.
     \warning only \c RTC_MAIN register is writeable, while the other are a copy of it and readonly. 
	 @param mode true for 12 hours mode, false for 24 hours display mode.
	 @see get1224Mode
	*/ 	
	void set1224Mode(const uint8_t target, const boolean mode);
	/**
	 \fn boolean isLeapYear(void)
	 \brief Returns if it is a leap year or not.
	 \return \c True if the year set in main clock is a leap year, \c False otherwise.
	 */
	boolean isLeapYear(void);
 	/**
	 \fn void setAlarmLevel(const uint8_t target, const uint8_t lvl)
	 \brief Sets the TTL level for MFP pin when the \c target alarm is triggered.
	 @param target can take one of the two vales: \c RTC_ALM0, \c RTC_ALM1 for alarm 0 and alarm 1, respectively.
	 @param lvl can take two values: \c HIGH or \c LOW.
	 @see getAlarmLevel
	 */
	void setAlarmLevel(const uint8_t target, const uint8_t lvl);
	/**
	 \fn uint8_t getAlarmLevel(const uint8_t target)
	 \brief Returns the TTL level of the MFP pin when the \c target alarm is triggered.
	 @param target can take one of the two vales: \c RTC_ALM0, \c RTC_ALM1 for alarm 0 and alarm 1, respectively.
	 @see setAlarmLevel
	 */
	inline uint8_t getAlarmLevel(const uint8_t target) { return (readByte(target+RTC_DAY) & 0x80)>>7; }
	/**
	 \fn void batterySupply(const boolean enable)
	 \brief Enables/Disables the external battery supply when main power fails.
	 @param enable setting this variable to true enables the external battery supply. Set it to false for disabling it.
	 */
	void batterySupply(const boolean enable);
    /**
     \fn void configureAlarmMode(const char *format,...)
     \brief configures what alarm (ALM0, ALM1, none, both) is active
     @param format character indicating the match criteria. Here are the admissible values:
     \arg \c 0 : sets only \c RTC_ALM0 as active
     \arg \c 1 : sets only \c RTC_ALM1 as active
     \arg \c b : sets both \c RTC_ALM0 and \c RTC_ALM1 as active
     \arg \c n : disables all alarms
     @see getAlarmMode, isAlarmActive
     */
    void configureAlarmMode(const char format);
    /**
     \fn boolean isAlarmActive(const uint8_t target);
     \brief returns whether the selected alarm is active.
     @param target can take one of the two values: \c RTC_ALM0, \c RTC_ALM1 for alarm 0 and alarm 1, respectively.
     @see getAlarmMode, configureAlarmMode
     */
    boolean isAlarmActive(const uint8_t target);
    /**
     \fn char getAlarmMode(void)
     \brief gets which alarm are active
     @return a character indicating which alarm mode is active; here are the possible values:
     \arg \c 0 : only \c RTC_ALM0 is active
     \arg \c 1 : only \c RTC_ALM1 is active
     \arg \c n : no alarms are active
     \arg \c b : both \c RTC_ALM0 and \c RTC_ALM1 are active
     */
   const char getAlarmMode(void);
    /**
     \fn void getConfBits(void)
     \brief prints values of some meaningful registers
     \warning since this reads some meaningful registers "unprotected", while not stopping the clock, it may be best not to use it or to stop and start the clock
     */
    void getConfBits(void);
    /**
     \fn void printConfBit(const uint_8t reg)
     \brief prints on serial port the conf bit relative to the register
     @param reg is the registry value
     \warning since this can read some meaningful registers "unprotected", while not stopping the clock, it may be best not to use it or to stop and start the clock
     */
    void printConfBit(const uint8_t reg);
	
	/**
	\fn uint8_t getTrimmingValue(void)
	\brief returns the contents of the oscillator trimming register
	@return the value of the trimming register 
	@see setTrimmingValueUnsigned, setTrimmingValueSigned
	*/
	inline uint8_t getTrimmingValue(void) {return readByte(RTC_MAIN+RTC_OSCTRIM);}
	
	/**
	\fn void setTrimming(uint8_t trimval)
	\brief sets the trimming register value, with bit 7 as the sign
	\param trimval represents the value to put in the register
	@see getTrimmingValue
	*/
	void setTrimming(uint8_t trimval);
	
	/**
	\fn void setSquareWaveOutput(uint8_t freqval)
	\brief configure the multifunction pin to output a certain frequency
	@param freqval can assume four values
	\arg \c 0 indicates a 32.768 kHz freq
	\arg \c 1 indicates a 8.192 kHz freq
	\arg \c 2 indicates a 4.096 kHz freq
	\arg \c 3 indicates a 1 Hz freq
	*/
	void setSquareWaveOutput(uint8_t freqval);
	
	/**
	\fn void clearSquareWaveOutput(void)
	\brief disables the square wave line output
	*/
	void clearSquareWaveOutput(void); 
	
	/**
	\fn char getStatusRegister(void)
	\brief gets the status of mem protection
	@returns a char indicating what part of memory is protected:
	\arg \c 0 means none
	\arg \c q means the upper quarter is protected
	\arg \c h means the upper half is protected
	\arg \c a means all of the eeprom is protected
	*/
	inline const char getStatusRegister(void){return _mem.getStatus();};
	/**
	\fn void writeArrayToEEprom(const uint8_t addr, uint8_t *data,uint8_t length)
	\brief Facade for inserting an array of data to the EEPROM, using writeBytesToMemory
	@param addr is the memory address. It should be between 0x00 and 0x7F, otherwise the counter will overflow
	@param data is an array of bytes to write onto the memory
	@param length is the length of the data to write, in general different from the array length
	@see writeBytesToMemory
	*/
	inline void writeArrayToEEprom(const uint8_t addr, uint8_t *data,uint8_t length){writeBytesToMemory(addr,data,length,true);}
	/**
	\fn void writeByteToEEprom(const uint8_t addr, uint8_t data)
	\brief write a single byte onto the RTC eeprom. It internally calls writeArrayToEEprom
	@param addr is the memory address. It should be between 0x00 and 0x7F, otherwise the counter will overflow
	@param data is the data to write
	@see writeArrayToEEprom, writeBytesToMemory
	*/
	inline void writeByteToEEprom(const uint8_t addr, uint8_t data){ writeArrayToEEprom(addr,&data,1);}
	/**
	\fn void readArrayFromEEprom(const uint8_t addr, uint8_t *data,uint8_t length)
	\brief reads a sequence of maximum 8 bytes from the RTC eeprom using readBytesFromMemory
	@param addr is the memory start address. It should be between 0x00 and 0x7F, otherwise the counter will overflow
	@param data is an array in which the data will be stored
	@param length is the length of the data to read
	@see readBytesFromMemory
	*/
	inline void readArrayFromEEprom(const uint8_t addr, uint8_t *data,uint8_t length){readBytesFromMemory(addr,data,length,true);}
	/**
	\fn uint8_t readByteFromEEprom(const uint8_t addr)
	\brief reads a single byte from the RTC eeprom using readSingleByteFromMemory
	@param addr is the memory start address. It should be between 0x00 and 0x7F, otherwise the counter will overflow
	@return the value stored in the memory
	@see readSingleByteFromMemory, readBytesFromMemory
	*/
	inline uint8_t readByteFromEEprom(const uint8_t addr){return readSingleByteFromMemory(addr,true);}
	/**
	\fn void writeArrayToSRAM(const uint8_t addr, uint8_t *data,uint8_t length)
	\brief Facade for inserting an array of data to the SRAM
	@param addr is the memory address. It must be between 0x20 and 0x5F. The counter won't overflow
	@param data is an array of data to write into the SRAM
	@param length is the number of bytes to write
	@see writeBytesToMemory
	*/
	 inline void writeArrayToSRAM(const uint8_t addr, uint8_t *data,uint8_t length){writeBytesToMemory(addr,data,length,false);}
	/**
	\fn void writeByteToSRAM(const uint8_t addr, uint8_t data)
	\brief writes a single byte to SRAM memory
	@param addr is the memory address
	@param data is the byte to write
	@see writeByteToSRAM, writeBytesToMemory
	*/
	 inline void writeByteToSRAM(const uint8_t addr, uint8_t data){ writeArrayToSRAM(addr,&data,1);}
	 /**
	 \fn void readArrayFromSRAM(const uint8_t addr, uint8_t *data,uint8_t length)
	 \brief facade for reading a set of bytes from the SRAM memory
	 @param addr the memory starting address
	 @param data the array on which the data will be written
	 @param length the number of bytes to read
	 @see readBytesFromMemory
	 */
	 inline void readArrayFromSRAM(const uint8_t addr, uint8_t *data,uint8_t length){readBytesFromMemory(addr,data,length,false);}
	/**
	\fn uint8_t readByteFromSRAM(const uint8_t addr)
	\brief reads a single byte from the SRAM memory
	@param addr the address from which to read
	@return the value stored in the SRAM
	@see readBytesFromMemory
	*/
	inline uint8_t readByteFromSRAM(const uint8_t addr){return readSingleByteFromMemory(addr,false);}
	
};

#endif
