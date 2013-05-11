/******************************************************
 SENSOR LIBRARY:
 
 Written by Enrico Formenti.  
 BSD license, check license.txt for more information
 All text above must be included in any redistribution.
 ******************************************************/

/*
 
 SENSOR.H: generic class for all sensors. Any other class should derive from this.
 
 */

/**
 \file Sensor.h
 \brief Definition of a virtual class for sensors.
 \details Header file containing the definition of a virtual class for sensors.
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
 \class Sensor Sensor.h
 \brief Abstract class for dealing with sensors.
 
 Abstract class for dealing with sensors.
 */

#ifndef SENSOR_H
#define SENSOR_H

#include "Component.h"
#include "SensorConstants.h"


class Sensor : public Component {
protected:
  /**
	 \fn virtual void read(void) = 0
	 \brief Virtual function for reading the internal status of the sensor and fill
	 the internal variables.
	 \remark In order to communicate data to the MCU, please use another function and name it \c getXXX, where
	 \c XXX is some meaning name for your data.
	 */
	virtual void read(void) = 0;  // read data from sensor
	
public:
	inline Sensor(uint8_t id, uint8_t t) : Component(id,t) { }
	/**
	 \fn virtual void begin(void) = 0
	 \brief Initializes the sensor.
	 */
	virtual void begin(void) = 0; // setup sensor internals
};
#endif
