#ifndef SHELLEXEC_H
#define SHELLEXEC_H

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>


class Shell_exec{
public :

	/*!
	 *  \brief Excute a shell command
	 *
 	 */	
	static std::string exec(std::string _cmd); 

	/*!
	 *  \brief Excute a shell command to scan BLE devices
	 *
 	 */	
	static std::string exec_scan_ble();

};


#endif