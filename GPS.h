#ifndef GPS_H
#define GPS_H


#include <string>
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <thread>
#include <mutex>
#include "myUart.h"

#define SIZE_HEADER_NMEA			6
#define KEY_START_GPGGA_FRAME		"$GPGGA"
#define KEY_START_GPVTG_FRAME		"$GPVTG"

#define PMTK_SET_BAUDRATE_115200	"$PMTK251,115200*1F"
#define PMTK_SET_FRAME_VTG_GGA 		"$PMTK314,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
#define PMTK_SET_NMEA_UPDATE_1HZ 	"$PMTK220,1000*1F"
#define PMTK_SET_FIX_CTL_1HZ 		"$PMTK300,1000,0,0,0,0*1C"

// Struct for the last data available from the GPS
typedef struct {
	std::string GPGGA;
	std::string GPVTG;
} GPSFrame;

// Struct for latitude and longitude
typedef struct {
	std::string value;
	char cardinalPoint;
} GeographicCoordinate;

// Struct for all GPS information
typedef struct {
	GeographicCoordinate latitude;
	GeographicCoordinate longitude;
	float altitude;
	float speed;
} GPSInfo;

class GPS 
{
private : 
	
	MyUart* uart;
	std::thread* collect_thread;
	std::mutex gpsFrameLock;
	GPSFrame gpsFrames;
	bool threadRunning = false;

	void task(void);
	void parseFrameBuffer (std::string& gpsInfo);

public :

	GPS(const std::string _uart);
	~GPS();

	int init(void);
	void sendCommand(std::string cmd);
	void start(void);
	void stop(void);
	void getGPSInfo(void);
};


#endif