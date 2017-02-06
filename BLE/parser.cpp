#include "parser.h"



#include "iostream"

Parser::Parser(){

}

bool Parser::checkSubString (std::string substr, std::string str){
	return ( str.find(substr) != std::string::npos ) ;
}

int Parser::parseNotification(std::string& notification, std::string& value){

	value = "";

	int pos = notification.find(MSG_NOTIFICATION);

	if(pos == std::string::npos){
		return -1; // wrong notification format
	}

	notification.erase(0, sizeof(MSG_NOTIFICATION)-1);

	std::regex regex("( [0-9a-fA-F][0-9a-fA-F])*");

	if(!std::regex_match(notification.c_str(), regex)){
		return -2;// wrong value format"
	}

	while(notification != ""){
		value += notification.substr(1, 2);
		notification.erase(0, 3);
	}

	return 0;
}
