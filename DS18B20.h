
/******************************************************
 SENSOR LIBRARY:
 
 Written by Enrico Formenti.  
 BSD license, check license.txt for more information
 All text above must be included in any redistribution.
 ******************************************************/



/**
 \file DS18B20.h
 \brief Class definition for DS18B20 component.
 \author Enrico Formenti
 \version 0.1
 \date 2012-2013
 \warning This software is provided "as is". The author is 
 not responsible for any damage of any kind caused by this
 software. Use it at your own risk.
 \copyright BSD license. See license.txt for more details. 
 All text above must be included in any redistribution. 
 */

/**
 \class DS18B20 DS18B20.h
 \brief Class for DS18B20 component. This a temperature sensor. 
 \details \brief Class for DS18B20 component. This a temperature sensor. The user is allowed to
 set an alarm which will be triggered whenever the temperature goes below \c _alarm_tmin
 or higher than \c _alarm_tmax. These bounds can be set using \c setAlarm function. Their range
 must be contained in the operating range of the component i.e. between -25°C and +125°C.
 */


#ifndef DS18B20_H
#define DS18B20_H

#include <Math.h>
#include "OWcomponent.h"
#include "Sensor.h"
#include "Error.h"
//#include <OneWire.h>

/* Sensor library
 
 DS18B20.H: class for temperature sensor DS18B20

written by Enrico Formenti
*/

#define CMD_READ_POWER_SUPPLY 0xB4
#define CMD_READ_SCRATCHPAD 0xBE
#define CMD_START_CONVERSION 0x44
#define CMD_WRITE_SCRATCHPAD 0x0

// Scratchpad locations
#define TEMP_LSB        0
#define TEMP_MSB        1
#define HIGH_ALARM_TEMP 2
#define LOW_ALARM_TEMP  3
#define CONFIGURATION   4
#define INTERNAL_BYTE   5
#define COUNT_REMAIN    6
#define COUNT_PER_C     7
#define SCRATCHPAD_CRC  8


class DS18B20: public Sensor, public OWcomponent {
private:
  uint8_t _pin;
	uint8_t _addr[8];
	uint8_t _datatmp[3];
	uint8_t _res;
//	float _resInc;
	int _alarm_tmin, _alarm_tmax;
	boolean _isAlarmOn;
	boolean _isAlarmTriggered;	
	boolean _isParasitePower;
	
	/**
	 \fn boolean getPowerSupplyMode(void)
	 \brief Returns \c True if this component is in parasite power mode (ie only two pins are used), \c False otherwise.
	 */
	boolean getPowerSupplyMode(void);

protected:
	uint8_t _scratchpad[8];
	/**
	 \fn void begin(void)
	 \brief Initializes the component internals. In particular, it obtains an address on the 1-Wire bus.
	 */
	void begin(void);
	
public:
	/**
	 \fn DS18B20(uint8_t p) : Sensor(S_DS18B20,ST_TEMPERATURE)  : OWcomponent(p)
	 \brief Constructor
	 @param p The Arduino pin to which the data pin of DS18B20 is connected to.
	 */
	inline DS18B20(uint8_t p) : Sensor(S_DS18B20,ST_TEMPERATURE), OWcomponent(p) { _isAlarmOn = false; };
	/**
	 \fn float getTemperature(void)
	 \brief Returns the temperature in Celsius degrees.
	 */
	float getTemperature(void);
	/**
	 \fn uint8_t getResolution(void)
	 \brief Returns the resolution of the temperature value.
	 \return Number of bits of resolution for the temperature value. Possible values are 9, 10, 11, 12.
	 \remark This number should be multiplied by the basic step value (0.0625) to get the actual approximation.
	 @see setResolution
	 */	
	inline uint8_t getResolution(void) { return _res; };
	/**
	 \fn uint8_t setResolution(uint8_t r)
	 \brief Sets the resolution of the temperature value.
	 \return r Number of bits of resolution for the temperature value. Admissible values are 9, 10, 11, 12. 
	 \remark This number should be multiplied by the basic step value (0.0625) to get the actual approximation in term of
	 degrees.
	 @see getResolution
	 */	
	void setResolution(uint8_t r);
	/**
	 \fn void setAlarm(int tmin, int tmax)
	 \brief Sets the temperature bound beyond or below which the alarm is triggered.
	 @param tmin If the temperature goes below this value the alarm is triggered. Minimal value is -55°C.
	 @param tmax If the temperature goes beyond this value the alarm is triggered. Maximal value is +125°C.
	 \remark Of course \c tmin has to be less or equal to \c tmax, otherwise an error is raised and the alarm is not set.
	 */
	void setAlarm(int tmin, int tmax);
	/**
	 \fn uint8_t getAlarmMin(void)
	 \brief Returns the inferior bound in temperature for the alarm trigger.
	 @see setAlarm, getAlarmMax.
	 */
	inline uint8_t getAlarmMin(void) { return _alarm_tmin; }
	/**
	 \fn uint8_t getAlarmMax(void)
	 \brief Returns the superior bound in temperature for the alarm trigger.
	 @see setAlarm, getAlarmMin.
	 */
	inline uint8_t getAlarmMax(void) { return _alarm_tmax; }
	/**
	 \fn void resetAlarm(void)
	 \brief Resets the current alarm flag. Use \c setAlarm to set a new alarm.
	 */
	inline void resetAlarm(void) { _isAlarmOn = false; }
	/**
	 \fn boolean isAlarmSet(void)
	 \brief Returns \c True if the alarm has been set, \c False otherwise.
	 @see isAlarmTriggered, resetAlarm, setAlarm.
	 */
	inline boolean isAlarmSet(void) { return _isAlarmOn; }
	/**
	 \fn boolean isAlarmTriggered(void)
	 \brief Return \c True if an alarm event has been triggered, \c False otherwise.
	 \remark The alarm keep being triggered until reset by \c resetAlarm.
	 @see isAlarmSet, setAlarm, resetAlarm.
	 */
	boolean isAlarmTriggered(void);	

	/**
	 \fn boolean isParasiteMode(void)
	 \brief Tests if the component is under parasite power supplies mode or not.
	 */
	inline boolean isParasiteMode(void) { return _isParasitePower; }
};

#endif
