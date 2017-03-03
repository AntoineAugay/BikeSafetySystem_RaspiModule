#ifndef APPLICATION_H
#define APPLICATION_H

#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#include <iomanip> // setprecision
#include <sstream> // stringstream
#include "BLE/ble_manager.h"
#include "GPS/GPS.h"



class Application{

private :
	
	
	GPS *gps;
	BLEManager *ble;
	
	GPSInfo gpsInfo;
	BLEInfo bleInfo;

	void serializeAllData(GPSInfo& gpsInfo, BLEInfo& bleInfo, std::string& out);
	std::string serialize(char header, float data);
	std::string serialize(char header, std::string data);
	std::string serialize(char header, bool data);	

public :

	/*!
	 *  \brief Application constructor 
	 *
 	 */	
	Application(std::string uartName);
	~Application();

	/*!
	 *  \brief start Application 
	 *
 	 */
	void start(void);


	
};


#endif