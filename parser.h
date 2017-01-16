#ifndef PARSER_H
#define PARSER_H

#include <string>

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

};


#endif