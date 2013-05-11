/******************************************************
 SENSOR LIBRARY:
 
 Written by Enrico Formenti.  
 BSD license, check license.txt for more information
 All text above must be included in any redistribution.
 ******************************************************/

/**
 \file DHT11.h
 \brief Class definition for DHT11 component.
 \author Enrico Formenti
 \version 0.1
 \date 2012-2013
 \warning This software is provided "as is". The author is 
 not responsible for any damage of any kind caused by this
 software. Use it at your own risk.
 \copyright BSD license. See license.txt for more details. 
 All text above must be included in any redistribution. 
 */

#ifndef DHT11_H
#define DHT11_H

#include "Sensor.h"
#include "Error.h"

/* Sensor library
 
 DHT11_H: class for DHT11 temperature/humidity sensor

written by Enrico Formenti
*/

/**
 \class DHT11 DHT11.h
 \brief Class for DHT11 component. This an humidity and temperature sensor.
 */

class DHT11 : public Sensor, public Error {
private:
  /**
	 \var uint8_t _pin
	 \brief Arduino pin to which the data pin of the DHT11 is connected to.
	*/
	uint8_t _pin;	
	uint8_t _data[5];
	uint32_t _lastreadtime;
	boolean firstreading;
protected:
	void read(void);
	
public:
	/**
	 \fn DHT11(uint8_t p) : Component(S_DHT11, ST_TEMPERATURE_HUMIDITY)
	 \brief Basic constructor. It calls super-class for defining id and type constants.
	 */
	inline DHT11(uint8_t p) : Sensor(S_DHT11, ST_TEMPERATURE_HUMIDITY) { _pin = p; }
	/**
	 \fn void begin(void)
	 \brief Initialization of sensor internals. This function should be called before the first \c get type access
	 to the sensor.
	 */
	void begin(void);
	/**
	 \fn float getTemperature(void)
	 \brief Reads the temperature value from the sensor.
	 \return The current value of the temperature in Celsius degrees.
	 */
	float getTemperature(void);
	/**
	 \fn float getHumidity(void)
	 \brief Reads the humidity value from the sensor.
	 \return The current value of the humidity.
	 */	
	float getHumidity(void);
};
#endif
