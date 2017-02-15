#include "Application.h"



Application::Application(std::string uartName){
	
	gps = new GPS(uartName);
	ble = new BLEManager();

	gpsInfo.hour = "00";
	gpsInfo.minut = "00";
	gpsInfo.altitude = 0.0;
	gpsInfo.speed = 0.0;
	gpsInfo.latitude.value = "000.00";
	gpsInfo.latitude.cardinalPoint = 'A';
	gpsInfo.latitude.value = "000.00";
	gpsInfo.latitude.cardinalPoint = 'A';

}

Application::~Application(){
	
	delete(gps);
	delete(ble);
	
}


void Application::start(void){


	timeval tvCurrentTime, tvLastSending;
	long currentTime = 0, lastSending = 0;

	ble->init();
	gps->init();

	ble->start();
	gps->start();

	while(42){

		
		gettimeofday(&tvCurrentTime, 0);
		long currentTime = (tvCurrentTime.tv_sec*1000) + (tvCurrentTime.tv_usec/1000);
		
		if(currentTime - lastSending > 1000){

			ble->getBLEInfo(bleInfo);
			std::cout << "rearModuleFind : " << bleInfo.rearModuleFind << std::endl;
			std::cout << "rearModuleWork : " << bleInfo.rearModuleWork << std::endl;
			std::cout << "commandModuleFind : " << bleInfo.commandModuleFind << std::endl;
			std::cout << "commandModuleWork : " << bleInfo.commandModuleWork << std::endl;

			
			if(gps->getGPSInfo(gpsInfo) != -1){;			
				std::cout << "Heure : " << gpsInfo.hour << ":" << gpsInfo.minut << std::endl;
				std::cout << "Speed : " << gpsInfo.speed << std::endl;
				std::cout << "Altitude : " << gpsInfo.altitude << std::endl;
				std::cout << "latitude : " << gpsInfo.latitude.value << "." << gpsInfo.latitude.cardinalPoint << std::endl;
				std::cout << "longitude : " << gpsInfo.longitude.value << "." << gpsInfo.longitude.cardinalPoint << std::endl;
			}

			
			//std::cout << "Time between sending : " << currentTime - lastSending << std::endl;
			gettimeofday(&tvLastSending, 0);
			long lastSending = (tvLastSending.tv_sec*1000) + (tvLastSending.tv_usec/1000);
			
		}
		
		usleep(100000);
	}

	ble->stop();	
	gps->stop();
}

int Application::serializeAllData(GPSInfo& gpsInfo, BLEInfo& bleInfo, std::string& out){
	/*
		header :
			- 0x01 : hour (2 octet)
			- 0x02 : minut (2 octet)
			- 0x03 : speed (N octet)
			- 0x11 : rearModuleFind (1 octet)
			- 0x12 : rearModuleWork (1 octet)
			- 0x13 : commandModuleFind (1 octet)
			- 0x14 : commandModuleWork (1 octet)
	*/

	out = "";

	if(gpsInfo.isValid == true){
		out += serialize((char)0x01, gpsInfo.hour);
		out += serialize((char)0x02, gpsInfo.minut);
		out += serialize((char)0x03, gpsInfo.speed);
	}

	out += serialize((char)0x11, bleInfo.rearModuleFind);
	out += serialize((char)0x12, bleInfo.rearModuleWork);
	out += serialize((char)0x13, bleInfo.commandModuleFind);
	out += serialize((char)0x14, bleInfo.commandModuleWork);
}


std::string Application::serialize(char header, float data){
	std::string tmp = "";
	std::string out = "";
	std::stringstream stream;

	tmp += header;

	stream << std::fixed << std::setprecision(1) << data;
	std::string s = stream.str();
	tmp += s;

	out += (char)tmp.size();
	out += tmp;

	//std::cout << (int)out.at(0) << (int)out.at(1) << out.substr(2,out.size()-2) << std::endl;
	return out;
}

std::string Application::serialize(char header, std::string data){
	std::string tmp = "";
	std::string out = "";

	tmp += header;
	tmp += data;

	out += (char)tmp.size();
	out += tmp;

	std::cout << (int)out.at(0) << (int)out.at(1) << out.substr(2,out.size()-2) << std::endl;
	return out;
}

std::string Application::serialize(char header, bool data){
	std::string tmp = "";
	std::string out = "";

	tmp += header;
	if(data = true){
		tmp += 'T';
	} else {
		tmp += 'F';
	}

	out += (char)tmp.size();
	out += tmp;

	std::cout << (int)out.at(0) << (int)out.at(1) << out.substr(2,out.size()-2) << std::endl;
	return out;
}