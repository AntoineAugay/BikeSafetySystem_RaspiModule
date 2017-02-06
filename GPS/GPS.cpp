#include "GPS.h"

GPS::GPS(const std::string _uart) { 	
	uart = new MyUart(_uart);
	gpsFrames.GPGGA = "";
	gpsFrames.GPVTG = "";
}

GPS::~GPS(){
	stop();
	uart->closeUART();
	std::cout << "End Tread" << std::endl;
	delete(collectThread);
}

int GPS::init(void){
	
	if(uart->initUART(9600) < 0){
		std::cout << "Uart opening issue" << std::endl;
		return -1;
	}
	
	sendCommand(PMTK_SET_BAUDRATE_115200);
	usleep(100000);
	uart->closeUART();

	if(uart->initUART(115200) < 0){
		std::cout << "Uart opening issue" << std::endl;
		return -1;
	}

	sendCommand(PMTK_SET_FRAME_VTG_GGA);
	return 0;
}

void GPS::start(void) {
	collectThread = new std::thread(&GPS::task, this);
}

void GPS::stop(void){
	threadRunning = false;
	collectThread->join();
}

void GPS::sendCommand(const std::string cmd) {
	std::string finalCmd(cmd);
	finalCmd += '\r';
	finalCmd += '\n';
	uart->writeUART(finalCmd.c_str());
}

void GPS::task(void) {

	bool threadRunning = true;
	char buffer[256];
	long int len, sleeptime;
	std::string frameBuffer = "";
	std::string delimiter = "$";
	std::size_t found1, foud2;

	std::cout << "GPS thread running" << std::endl;

	while(threadRunning){

		len = uart->readUART(buffer);
		if(len > 0) {
			std::cout  << "<" << buffer << ">" << std::endl;
			sleeptime = 990000;
		} else {
			sleeptime = 200000;
			continue;
		}

		frameBuffer += buffer;
		parseFrameBuffer(frameBuffer);
		usleep(sleeptime);
	}
}

void GPS::parseFrameBuffer (std::string& gpsInfo) {

	std::string delCRLF = "\r\n";
	std::size_t foundGGA, foundVTG, foundEnd;

	// Search NMEA header, here GPGGA and GPVTG
	foundGGA = gpsInfo.find(KEY_START_GPGGA_FRAME);
	foundVTG = gpsInfo.find(KEY_START_GPVTG_FRAME);

	// If the beginning of gppInfo isn't '$GPGGA' or '$GPVTG'
	if( (foundGGA == std::string::npos) 
		&& (foundVTG == std::string::npos))
	{
		// Clear the buffer
		gpsInfo = "";
	} else if( foundGGA != 0 && foundVTG != 0) {
		// Clear all information before
		gpsInfo.erase(0, std::min(foundGGA, foundVTG));
	}

	// Search the end of a frame (CR+LF)
	foundEnd = gpsInfo.find(delCRLF);
	// while there is a complete frame
	while( (gpsInfo != "") && (foundEnd != std::string::npos)){
		
		std::string frame = gpsInfo.substr(0, foundEnd);
		
		std::cout << "Extrated frame : " << frame ;

		std::string header = gpsInfo.substr(0, SIZE_HEADER_NMEA);

		gpsFrameLock.lock();
		if(header == KEY_START_GPGGA_FRAME) {
			gpsFrames.GPGGA = frame;
		} else if (header == KEY_START_GPVTG_FRAME){
			gpsFrames.GPVTG = frame;
		} else {
			std::cout << " -> Invalid frame" << std::endl;
		}
		gpsFrameLock.unlock();

		gpsInfo.erase(0, foundEnd+delCRLF.size());
		foundEnd = gpsInfo.find(delCRLF);
	}
}

int GPS::getGPSInfo(GPSInfo& gpsInfo) {
	std::string GGAFrame = gpsFrames.GPGGA;
	std::string VTGFrame = gpsFrames.GPVTG;
	std::regex regex;
	std::vector<std::string> result;


	// Get parameter of VTGFrame
	strToVector(VTGFrame, result);

	if(	result[0].compare("$GPVTG") 
		|| result[8].compare("K")) {
		return -1;
	}

	// Get speed
	regex = "[0-9]+.[0-9]+";
	if(std::regex_match(result[7].c_str(), regex)) {
		gpsInfo.speed = atof(result[7].c_str());
	} else {
		return -2;
	}

	// Get parameter of GGAFrame
	strToVector(GGAFrame, result);


	if( result[0].compare("$GPGGA")
		|| result[10].compare("M")
		|| result[12].compare("M")){
		return -1;
	}

	// Get time
	regex = "[0-9]{6}.[0-9]{3}";
	if(std::regex_match(result[1].c_str(), regex)) {
		gpsInfo.hour = atoi(result[1].substr(0,2).c_str());
		gpsInfo.minut = atoi(result[1].substr(2,2).c_str());
	} else {
		return -2;
	}

	// Get latitude value
	regex = "[0-9]+.[0-9]+";
	if(std::regex_match(result[2].c_str(), regex)) {
		gpsInfo.latitude.value = result[2].c_str();
	} else {
		return -2;
	}

	
	// Get latitude cardinal direction
	regex = "N|S";
	if(std::regex_match(result[3].c_str(), regex)) {
		const char* tmp = result[3].substr(0,1).c_str();
		gpsInfo.latitude.cardinalPoint = tmp[0];
	} else {
		return -2;
	}

	// Get longitude value
	regex = "[0-9]+.[0-9]+";
	if(std::regex_match(result[4].c_str(), regex)) {
		gpsInfo.longitude.value = result[4];
	} else {
		return -2;
	}

	// Get longitude cardinal point
	regex = "E|W";
	if(std::regex_match(result[5].c_str(), regex)) {
		const char* tmp = result[5].substr(0,1).c_str();
		gpsInfo.longitude.cardinalPoint = tmp[0];
	} else {
		return -2;
	}

	// Get Altitude
	regex = "[0-9]+.[0-9]+";
	if(std::regex_match(result[9].c_str(), regex)) {
		gpsInfo.altitude = atof(result[9].c_str());
	} else {
		return -2;
	}

	// Get Sea level
	regex = "[0-9]+.[0-9]+";
	if(std::regex_match(result[11].c_str(), regex)) {
		gpsInfo.seaLevel = atof(result[11].c_str());
	} else {
		return -2;
	}

	return 0;
}

void GPS::strToVector(std::string str, std::vector<std::string>& result) {

	std::stringstream strStream(str);
	std::string parsed;
	int i = 0;
	result.clear();

	while(getline(strStream,parsed,','))
	{
	     result.push_back(parsed);
	}
}