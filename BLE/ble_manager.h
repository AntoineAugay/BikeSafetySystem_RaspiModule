#ifndef BLEMANAGER_H
#define BLEMANAGER_H


#include <iostream>
#include <string>
#include <exception>
#include <thread>
#include <sys/time.h>
#include <unistd.h>

#include "parser.h"
#include "shell_exec.h"
#include "String_queue.h"


#define SEND_CHARAC 0x000b
#define RECEIVE_CHARAC 0x000d
#define CONF_CHARAC 0x000e


enum ModuleType{
	REAR,
	COMMAND
};

class BLEManager{
private :
	
	Parser parser;
	
	bool isInitDone;
	bool isRearModuleActive;
	bool isCommandModuleActive;

	std::string rearModuleAddr;
	std::string commandModuleAddr;

	std::thread* BLEThread;
	bool threadRunning;

	StringQueue MessagesToRear;
	StringQueue MessagesToCommand;

	/*!
	 *  \brief Thread function 
	 *	
 	 */
	void task(void);


	

public :

	/*!
	 *  \brief BLEManager constructor 
	 *
 	 */	
	BLEManager()
	{}


	/*!
	 *  \brief start BLEThread 
	 *	
 	 */
	int start(void);
	

	/*!
	 *  \brief stop BLEThread  
	 *	
 	 */
	void stop(void);


	/*!
	 *  \brief initialization of the bluetooth BLE 
	 *	
	 *	\return -1 : No dongle BLE detected
 	 */
	int init(void);

	/*!
	 *  \brief collect rear module and command address adress
	 *	
	 *	
	 *	\return -1 : command module not detected
	 *	\return -2 : rear module not detected
	 *	\return -3 : No module detected
 	 */
	int getModuleAddr();

	/*!
	 *  \brief collect message from  a device 
	 *
	 *	\param module : module name
	 *	\param frame : message from device
	 *	
	 *	\return -1 : param "module" wrong
	 *	\return -2 : Module isn't running
	 *	\return -3 : Parsing error
	 *	\return -4 : Parsing error
 	 */
	int getMessageFromDevice(const ModuleType module, std::string& frame);

	/*!
	 *  \brief send a message to a device 
	 *
	 *	\param addr : address of the device
	 *	\param message : message tosend
 	 */
	int sendMessageToDevice(const ModuleType module, std::string message);

	/*!
	 *  \brief parse frame to process them 
	 *
	 *	\param frame
	 *	\return -1 : frame is empty
 	 */
	int parseCommandFrame(std::string frame);

};


#endif