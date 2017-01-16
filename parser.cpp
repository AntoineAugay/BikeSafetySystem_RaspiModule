#include "parser.h"

Parser::Parser(){

}

bool Parser::checkSubString (std::string _substr, std::string _str){
	return ( _str.find(_substr) != std::string::npos ) ;
}
