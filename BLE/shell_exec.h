#ifndef SHELLEXEC_H
#define SHELLEXEC_H

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>


class ShellExec{
public :

	/*!
	 *  \brief Excute a shell command
	 *
 	 */	
	static std::string exec(std::string cmd); 

	/*!
	 *  \brief Excute a shell command to scan BLE devices
	 *
 	 */	
	static std::string execScan();

	/*!
	 *  \brief Excute a shell command to connect devices
	 *
 	 */	
	static std::string execGetBleNotification(std::string addr, int timeout_ms);

};


#endif