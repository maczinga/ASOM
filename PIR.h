/**
 \file PIR.h
 \brief Definition of the PIR class.
 \details Header file containing the definition of the PIR class (generic PIR sensor).
 \author Enrico Formenti
 \version 0.1
 \date 2012-2013
 \warning This software is provided "as is". The author is 
 not responsible for any damage of any kind caused by this
 software. Use it at your own risk.
 \copyright BSD license. See license.txt for more details. 
 All text above must be included in any redistribution. 
*/

#ifndef PIR_H
#define PIR_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Sensor.h"


/**
 \class PIR PIR.h
 \brief A generic class for PIR movement sensors.
 
 This class implements a generic PIR sensor with one pin for output.
 */

/**
 \example PIRtest/PIRtest.ino 
 This gives an example of usage the PIR movement sensor.
 Remark that initialisation of the sensor may take some time. (See the function \c setInitTime in class \c PIR).
 */  


class PIR : public Sensor {
private:

	uint8_t _pin;
	uint16_t _init_delay;
	uint16_t _duration_delay;
	
	inline void read(void) {}
	
public:
	
	/**
	 \fn PIR(const uint8_t p) : Sensor(GENERIC_PIR,ST_PIR)
	 \brief Constructor
	 @param p Arduino pin to which the pin of the sensor is connect.
	*/
	inline PIR(const uint8_t p) : Sensor(S_GENERIC_PIR,ST_PIR) { _pin = p; _init_delay = 17000; _duration_delay = 5000; }
	/**
	 \fn setDurationDelay(uint16_t d)
	 \brief Sets the value of the delay which the sensor waits before sensing next event.
	 @param d Delay
	 */
	inline void setDurationDelay(uint16_t d) { _duration_delay = d; }
	/**
	 \fn getDurationDelay(uint16_t d)
	 \brief Returns the value of the delay which the sensor waits before sensing next event.
	 \return The value of the delay
	 */	
	inline uint16_t getDurationDelay(void) { return _duration_delay; }
	/**
	 \fn setInitDelay(uint16_t d)
	 \brief Sets the delay which the sensor takes for its initialization.
	 @param d Delay
	 */
	inline void setInitDelay(uint16_t d) { _init_delay = d; }
	/**
	 \fn getInitDelay(void)
	 \brief Return the delay which the sensor takes for its initialization.
	 @param d the value of the delay
	 */
	inline uint16_t getInitDelay(void) { return _init_delay; }
	/**
	 \fn begin(void)
	 \brief Initializes the sensor.
	 */	
	inline void begin(void) { pinMode(_pin, INPUT ); delay(_init_delay+_duration_delay); }
	/**
	 \fn uint8_t getStatus(void)
	 \brief Returns the alarm status of the sensor (1 = MOUVEMENT SENSED, 0 = NO MOVEMENT SENSED).
	 */
	inline uint8_t getStatus(void) { return digitalRead(_pin); }
};

#endif
