#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "my_exception.h"
#include <regex>


#define MSG_CHAR_VALUE_WRITTEN "Characteristic value was written successfully"
#define MSG_NOTIFICATION "Notification handle = 0x000d value:"


class Parser{

public :

	/*!
	 *  \brief BLE_manager constructor 
	 *
 	 */	
	Parser();
	
	/*!
	 *  \brief Check if substr is in str 
	 *  
	 *	\param substr : string to find in str
	 *	\param str : string 
	 *	\return true if substr is in str otherwise return false
 	 */	
	bool checkSubString (std::string _substr, std::string _str); 

	/*!
	 *  \brief Check if substr is in str 
	 *  
	 *	\param _notification : string to analyse
	 *	\return true if substr is in str otherwise return false
 	 */	
	std::string parse_notification(std::string& _notification);
};


#endif