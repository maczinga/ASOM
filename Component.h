/**
 \file Component.h
 \brief Class definition for a generic component.
 \details This class implements a generic component. All other classes should inherit from it. It implements basic error
 management and component identification interface.
 \author Enrico Formenti
 \version 0.1
 \date 2012-2013
 \warning This software is provided "as is". The author is 
 not responsible for any damage of any kind caused by this
 software. Use it at your own risk.
 \copyright BSD license. See license.txt for more details. 
 All text above must be included in any redistribution. 
*/

#ifndef COMPONENT_H
#define COMPONENT_H

/**
 \class Component Component.h
 \brief Class for a generic component.
 
 This class implements a generic component. Basically, it implements error management and component identification.
 All other classes should inherit from this one.
 
 */

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Component {
protected:
  /**
	 \var uint32_t _id
	 \brief identifier for the component.
	 */	
	uint32_t _id;
	/**
	 \var uint32_t _type
	 \brief Identifier for the type of component.
	 */	
	uint32_t _type;
	
public:
	/**
	 \fn Component(uint32_t i, uint32_t t)
	 \brief Constructor for a generic component.
	 @param i Identifier of the component.
	 @param t Type of the component (temperature sensor, humidity sensor, real time clock, etc.)
	 */	
	inline Component(uint32_t i, uint32_t t) { _id = i; _type = t; }
	/**
	 \fn uint32_t getId(void)
	 \brief Gets the identifier for this component.
	*/
	inline uint32_t getId(void) { return _id; }
	/**
	 \fn uint32_t getType(void)
	 \brief Gets the type of this component.
	 */
	inline uint32_t getType(void) { return _type; }
	/**
	 \fn boolean isComponent(uint32_t c, uint32_t t)
	 \brief Tests if this component is the same as \c c.
	 @param c Identifier of the component.
	 @param t Type of the component (temperature sensor, humidity sensor, real time clock, etc.)	 
	 */
	inline boolean isComponent(uint32_t c, uint32_t t) { return (_id == c)&&(_type == t); }
};

#endif
