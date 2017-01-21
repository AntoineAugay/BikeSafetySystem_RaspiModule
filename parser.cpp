#include "parser.h"



#include "iostream"

Parser::Parser(){

}

bool Parser::checkSubString (std::string _substr, std::string _str){
	return ( _str.find(_substr) != std::string::npos ) ;
}

std::string Parser::parse_notification(std::string& _notification){

	std::string result ("");

	int pos = _notification.find(MSG_NOTIFICATION);

	if(pos == std::string::npos){
		throw myException(5, "parse_notification() : wrong notification format", 1);
	}

	_notification.erase(0, sizeof(MSG_NOTIFICATION)-1);

	std::regex regex("( [0-9a-fA-F][0-9a-fA-F])*");

	if(!std::regex_match(_notification.c_str(), regex)){
		throw myException(6, "parse_notification() : wrong value format", 1);
	}

	while(_notification != ""){
		result += (char) strtoul(_notification.substr(1, 2).c_str(), NULL, 16);
		_notification.erase(0, 3);
	}

	return result;
}
