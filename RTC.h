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
 \version 0.1
 \date 2012-2013
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

#include "I2Ccomponent.h"
#include "Component.h"
#include "Error.h"

#define RTC_SECOND 0x0
#define RTC_MINUTE 0x01
#define RTC_HOUR 0x02
#define RTC_DAY 0x03
#define RTC_DATE 0x04
#define RTC_MONTH 0x05
#define RTC_YEAR 0x06

#define RTC_ALM_I_FLAG 0x08
#define RTC_1224_FLAG 0x40
#define RTC_ALM_LVL_FLAG 0x80

#define RTC_ALM_CFG 0x03
#define RTC_CONFIGURATION_BYTE 0x7
#define RTC_ALM0_CONFIGURATION_BYTE 0x0D
#define RTC_ALM1_CONFIGURATION_BYTE 0x14

/**
 \class RTC RTC.h
 \brief A class for real time clock module based on MCP79410 chip.
 
 This class...
 */

class RTC : public I2Ccomponent, public Component, public Error {
private:

  /**
	 \var uint8_t _mfppin
	 \brief Arduino pin to which the MFP pin of the RTC is connected.
	 */
	uint8_t _mfppin;
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
	static const uint8_t RTC_ALM0;
	/**
	 \var static const uint8_t RTC_ALM1
	 \brief Address of alarm 1.
	 \warning Use the variable in your programs. Direct use of the value \c 0x11 might result incompatible with future versions.
	 */
	static const uint8_t RTC_ALM1;
	
	/**
	 \fn RTC(const uint8_t mfp) : I2Ccomponent(0x6F)
	 \brief Constructor
	 @param mfp Arduino pin to which the MFP pin is connect.
	*/
	inline RTC(const uint8_t mfp) : I2Ccomponent(0x6F), Component(1,1) { _mfppin = mfp; }
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
	 /**
	 \example RTCtest/RTCtest.ino 
	  This gives an example of usage for the functions \c setDate(), \c setTime(), \c getDate(), \c getTime().
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
	inline boolean isAlarmTriggered(const uint8_t target) { return (boolean)(readByte(target+RTC_DAY) & 0x08)>>3; }
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
	 \arg \c a : matches seconds, minutes, hours, day, date, month.
	 \arg \c A : same as \c a
	 \n
     \remark Parameters are processed according to the order of appearence in \c format.	 
	 @see isAlarmTriggered, alarmFlagReset
	*/
	void setAlarmMatch(const uint8_t target, const char format);
	/**
	 \fn void getAlarmMatch(const uint8_t target)
	 \brief Gets the criteria used by the module for triggering the alarm \c target.
	 @param target can take one of the two vales: \c RTC_ALM0, \c RTC_ALM1 for alarm 0 and alarm 1, respectively.
	 @return a character indicating the match criteria. Here are the possible values:
	 \arg \c s : seconds
	 \arg \c m : minutes
	 \arg \c h : hours
	 \arg \c d : day (alarm triggered at 12:00:00 AM)
	 \arg \c a : matches seconds, minutes, hours, day, date, month.
	 \n
	 @see setAlarmMatrch, isAlarmTriggered, alarmFlagReset
	 */	
	uint8_t getAlarmMatch(const uint8_t target);
	/**
	 \fn boolean get1224Mode(const uint8_t target)
	 \brief Returns the display mode for the target clock or alarm.
	 @param target can take one of the three vales: \c RTC_MAIN, \c RTC_ALM0, \c RTC_ALM1 for main clock, alarm 0 and alarm 1, respectively.
	 \return \c True if the target clock/alarm is in 12 hours display mode, \c False for 24 hours mode.
	 @see set1224Mode
	 */ 
	inline boolean get1224Mode(const uint8_t target) { return (boolean)(readByte(target+RTC_HOUR) & 0x40)>>5; }
	/**
	 \fn void set1224Mode(const uint8_t target, const boolean mode)
	 \brief Sets the clock display mode for the given target clock or alarm.
	 @param target can take one of the three vales: \c RTC_MAIN, \c RTC_ALM0, \c RTC_ALM1 for main clock, alarm 0 and alarm 1, respectively.
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
};

#endif
