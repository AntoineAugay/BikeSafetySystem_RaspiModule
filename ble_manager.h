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
	BLE_manager()
	{}

	/*!
	 *  \brief initialization of the bluetooth BLE 
	 *
 	 */
	void initialization(void);

	/*!
	 *  \brief collect rear module and command address adress
	 *	
	 * \param _addr_rear_module : string which store rear module address
	 * \param _addr_rear_module : string which store command module address
 	 */
	void get_module_addr(std::string& _addr_rear_module,std::string& _addr_command_module);

	/*!
	 *  \brief collect message from  a device 
	 *
	 *	\param addr : address of the device
 	 */
	std::string get_message_from_device(const std::string& _addr);

	/*!
	 *  \brief send a message to a device 
	 *
	 *	\param addr : address of the device
	 *	\param message : message tosend
 	 */
	void send_message_to_device(const std::string& _addr, std::string _message);
};


#endif