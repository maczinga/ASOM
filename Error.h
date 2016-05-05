/**
 \file Error.h
 \brief Class definition for error management.
 \details This class implements very basic error management common to all components.
 \author Enrico Formenti
 \author Daniele Ratti
 \version 0.5
 \date 2012-2013; 2016
 \warning This software is provided "as is". The author is 
 not responsible for any damage of any kind caused by this
 software. Use it at your own risk.
 \copyright BSD license. See license.txt for more details. 
 All text above must be included in any redistribution. 
*/

#ifndef ERROR_H
#define ERROR_H
/**
 \name Error codes
 \brief Macro definitions for the error codes. All possible errors are collected here for compactness and reuse sake. 
 \remarks The codes are defined as 4 bytes sequences and are randomly generated.
 */

//@{

/**
 \def ERROR_NONE 0x0
 \brief Signals that that no error has occurred.
 */
#define ERROR_NONE 0x0
/**
 \def ERROR_INVALID_CRC 0xb9e5
 \brief Signals that a CRC check has failed.
 */
#define ERROR_INVALID_CRC 0xb9e5
/**
 \def ERROR_NO_MORE_ADDRESSES 0x52aa
 \brief Signals that no more addresses can be attributed. 
 \details Signals that no more addresses can be attributed. This can happen when using components which use the 1-Wire bus, 
 for example.
 */
#define ERROR_NO_MORE_ADDRESSES 0x52aa
/**
 \def ERROR_TIME_OUT 0xab70
 \brief Signals that the last request has timed out. 
 */
#define ERROR_TIME_OUT 0xab70
/**
 \def ERROR_READ_FAILURE 0xca07
 \brief Signals that the last read has failed for some reason. 
 */
#define ERROR_READ_FAILURE 0xca07
/**
 \def ERROR_WRITE_FAILURE 0xc807
 \brief Signals that the last write has failed for some reason. 
 */
#define ERROR_WRITE_FAILURE 0xc807
/**
 \def ERROR_OUT_OF_RANGE 0x5437
 \brief Signals that the value obtained or passed as parameter is out of allowed range. 
 */
#define ERROR_OUT_OF_RANGE 0x5437
/**
 \def ERROR_INVALID_DATA 0xae92d210
 \brief Signals that invalid data has been detected. 
 */
#define ERROR_INVALID_DATA 0xae92d210
/**
 \def ERROR_INVALID_FORMAT 0xa67ea6fe
 \brief Signals that the data has not the expected format/structure. 
 */
#define ERROR_INVALID_FORMAT 0xa67ea6fe
//@}

/**
 \class Error Error.h
 \brief Class for basic error management.
 
 This class implements very basic error management.
 All other component classes should inherit from this one. 
 */

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Error {
  
private:
	/**
	 \var uint16_t _error
	 \brief Code of the last error or \c ERROR_NONE if no error occurred.
	 */	
	uint16_t _error;
	
public:
	/**
	 \fn Error(void)
	 \brief Basic constructor.
	 */
	inline Error(void) { _error = ERROR_NONE; }
	/**
	 \fn Error(uint16_t e)
	 \brief Basic constructor.
	 @param e Code of the error occurred. See the macro definitions for error codes. 
	 \remark Please use the macros for error codes in order to be compatible with future versions of this software.
	 */
	
	inline Error(uint16_t e) { _error = e; }
	/**
	 \fn uint8_t getError(void)
	 \brief Returns the code of the last error occurred or \c ERROR_NONE for no errors.
	*/
	inline uint16_t getError(void) { return _error; }
	/**
	 \fn uint8_t setError(uint16_t e)
	 \brief Sets the code for the error just occurred (or \c ERROR_NONE for no errors for resetting error status).
	 @param e Code of error that has occurred.
	 */	
	inline uint16_t setError(uint16_t e) { _error = e; }
	/**
	 \fn bool hasErrorOccurred(void)
	 \brief Returns \c True if an error has occurred, \c False otherwise.
	 */
	inline bool hasErrorOccurred(void) { return _error != ERROR_NONE; }
	/**
	\fn void clearError(void)
	\brief reset the error to \c ERROR_NONE 
	*/
	inline void clearError(void){_error=ERROR_NONE;}
};

#endif
