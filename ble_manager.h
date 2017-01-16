#ifndef BLEMANAGER_H
#define BLEMANAGER_H


#include <iostream>
#include <string>

#include "parser.h"
#include "shell_exec.h"
#include "my_exception.h"


#define SEND_CHARAC 0x000b
#define RECEIVE_CHARAC 0x000d
#define CONF_CHARAC 0x000e



class BLE_manager{
private :
	
	Parser parser;


public :

	/*!
	 *  \brief BLE_manager constructor 
	 *
 	 */	
	BLE_manager();
	~BLE_manager();


	/*!
	 *  \brief initialization of the bluetooth BLE 
	 *
 	 */
	void initialization(void);

	void getModuleAddr(std::string& _addr_rear_module,std::string& _addr_command_module);

};


#endif