#include "ble_manager.h"


int BLEManager::start(void){
	if(isInitDone){
		std::cout << "thread Running" << std::endl;
		BLEThread = new std::thread(&BLEManager::task, this);
		return 0 ;	
	} else {
		return -1;
	}
}

void BLEManager::stop(void){
	threadRunning = false;
	BLEThread->join();
	std::cout << "thread stopped" << std::endl;
}


void BLEManager::task(void){
	threadRunning = true;	

	timeval tvCurrentTime, tvLastScan;
	long int currentTime = 0;
	long int lastScan = 0;

	while(threadRunning) {

		// Scan to find REAR module and COMMAND module
		if(rearModuleAddr == "" || commandModuleAddr == "") {

			

			gettimeofday(&tvCurrentTime, 0);
			currentTime = (tvCurrentTime.tv_sec*1000) + (tvCurrentTime.tv_usec/1000);
			
			if( currentTime - lastScan > 5000 ){

				std::cout << "REAR : " << rearModuleAddr << " / COMMAND : " << commandModuleAddr << std::endl;
				getModuleAddr();
				gettimeofday(&tvLastScan, 0);
				lastScan = (tvLastScan.tv_sec*1000) + (tvLastScan.tv_usec/1000);
			}

		} else {

			std::string message = "";

			// Send message to rear module
			message = MessagesToRear.pop();
			if(message != "") {
				sendMessageToDevice(REAR, message);
			}

			// Here to send message to command module (not usefull now)
			
			// Get message from command module
			std::string frame = "";
			getMessageFromDevice(COMMAND, frame);
			
			if(frame != ""){
				parseCommandFrame(frame);
			}

			// Here to receive message from rear module (not usefull now)
		}

	}
	usleep(100);
}

int BLEManager::init(void){
	isInitDone = false;
	std::string result = ShellExec::exec("hciconfig");

	if(parser.checkSubString("DOWN", result)) {
		result = ShellExec::exec("hciconfig");
		std::cout << "Dongle BLE DOWN..." << std::endl;
		ShellExec::exec("sudo hciconfig hci0 up");

		result = ShellExec::exec("hciconfig");
	}

	if(parser.checkSubString("UP RUNNING", result)) {
		std::cout << "Dongle BLE RUNNING" << std::endl;
		isInitDone = true;
		return 0;
	} else {
		return -1; // No dongle BLE detected
	}
}

int BLEManager::getModuleAddr(){
	
	std::string line = "";
	std::string delimiter = "\n";
	std::string result;
	size_t pos = 0;
	
	/* Execute scan Shell command and collect result */
	result = ShellExec::execScan();

	/* Process each line of result to find module addresses */
	while ((pos = result.find(delimiter)) != std::string::npos) {
	    line = result.substr(0, pos);
	    result.erase(0, pos + delimiter.length());

	    if(parser.checkSubString("REAR", line)){
	    	rearModuleAddr = line.substr(0, 17);
	    	isRearModuleActive = true;
	 	} else {
	 		isRearModuleActive = false;
	 	}

	 	if(parser.checkSubString("COMMAND", line)){
			commandModuleAddr = line.substr(0, 17);
			isCommandModuleActive = true;
	 	} else {
	 		isCommandModuleActive = false;
	 	}
	}

	
	if(rearModuleAddr == "" && commandModuleAddr == ""){
		return -3; // No module detected
	} else if (rearModuleAddr == "") {
		return -2; // rear module not detected
	} else if (commandModuleAddr == "") {
		return -1; // command module not detected
	} else {
		return 0;
	}
}

int BLEManager::getMessageFromDevice(const ModuleType module, std::string& frame){

	int err = 0;
	std::string moduleAddr;


	switch(module) {
		case REAR : 
			moduleAddr = rearModuleAddr;
			break;
		case COMMAND :
			moduleAddr = commandModuleAddr;
			break;
		default:
			return -1;
	}

	std::string cmd_result = ShellExec::execGetBleNotification(moduleAddr, 0);

	frame = ""; 

	if(cmd_result == ""){
		
		switch(module) {
			case REAR : 
				isRearModuleActive = false;
				break;
			case COMMAND :
				isCommandModuleActive = false;
				break;
		}

		return -2; // Module isn't running
	} else {
		switch(module) {
			case REAR : 
				isRearModuleActive = true;
				break;
			case COMMAND :
				isCommandModuleActive = true;
				break;
		}
	}


	std::string delimiter = "\n";
	std::string line;
	int pos;

	// Get the end of the first
	pos = cmd_result.find(delimiter);
	
	if(pos == std::string::npos){
		return-3; // Parsing error
	}

	// Extract the first line
	line = cmd_result.substr(0, pos);
	cmd_result.erase(0, pos + delimiter.length());

	// Check if the first line is the one which we expect
	if(!parser.checkSubString(MSG_CHAR_VALUE_WRITTEN, line)){
		return -4; // Parsing error
	} 

	// Analyse each line and extract data
	while ((pos = cmd_result.find(delimiter)) != std::string::npos){
		std::string value = "";
		line = cmd_result.substr(0, pos-1);
		cmd_result.erase(0, pos + delimiter.length());
		// Extract data 
		err = parser.parseNotification(line, value);
		if(err == 0){
			frame += value;
		}
	}

	return 0;
}

int BLEManager::sendMessageToDevice(const ModuleType module, std::string message){
	
	std::string moduleAddr;
	switch(module) {
		case REAR : 
			moduleAddr = rearModuleAddr;
			break;
		case COMMAND :
			moduleAddr = commandModuleAddr;
			break;
		default:
			return -1;
	}
}

int BLEManager::parseCommandFrame(std::string frame){

	if(frame == ""){
		return -1;
	}

	while(frame != ""){
		unsigned int header;   
		std::stringstream ss;
		ss << std::hex << frame.substr(0, 2);
		// (char) strtoul(notification.substr(1, 2).c_str(), NULL, 16);
		ss >> header;

		switch(header){
		case 0x01:
			MessagesToRear.push(frame.substr(0, 2));
			std::cout << frame.substr(0, 2) << " added to MessagesToRear" << std::endl;
			break;
		case 0x02:
			MessagesToRear.push(frame.substr(0, 2));
			std::cout << frame.substr(0, 2) << " added to MessagesToRear" << std::endl;
			break;
		}	
		frame.erase(0,2);
	}
	
	return 0;

}