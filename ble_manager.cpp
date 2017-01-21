#include "ble_manager.h"


void BLE_manager::initialization(void){

	std::string result = Shell_exec::exec("hciconfig");

	if(parser.checkSubString("DOWN", result)) {
		result = Shell_exec::exec("hciconfig");
		std::cout << "Dongle BLE DOWN..." << std::endl;
		Shell_exec::exec("sudo hciconfig hci0 up");

		result = Shell_exec::exec("hciconfig");
	}

	if(parser.checkSubString("UP RUNNING", result)) {
		std::cout << "Dongle BLE RUNNING" << std::endl;
	} else {
		throw myException(1, "initialization() : No dongle BLE detected", 0);
	}
}

void BLE_manager::get_module_addr(std::string& _addr_rear_module,std::string& _addr_command_module){
	
	std::string line = "";
	std::string delimiter = "\n";
	std::string result;
	size_t pos = 0;
	
	/* Execute scan Shell command and collect result */
	result = Shell_exec::exec_scan();
	
	/* Initialisation of addr */
	_addr_command_module = "";
	_addr_rear_module = "";

	/* Process each line of result to find module addresses */
	while ((pos = result.find(delimiter)) != std::string::npos) {
	    line = result.substr(0, pos);
	    result.erase(0, pos + delimiter.length());

	    if(parser.checkSubString("REAR", line)){
	    	_addr_rear_module = line.substr(0, 17);
	 	}

	 	if(parser.checkSubString("COMMAND", line)){
			_addr_command_module = line.substr(0, 17);
	    }
	}

	if(_addr_rear_module == "" && _addr_command_module == ""){
		throw myException(2, "getModuleAddr() : Modules not detected", 0);
	} else if (_addr_rear_module == "") {
		throw myException(3, "getModuleAddr() : Rear module not detected", 1);
	} else if (_addr_command_module == "") {
		throw myException(4, "getModuleAddr() : Command module not detected", 1);
	}
}

std::string BLE_manager::get_message_from_device(const std::string& _addr){

	std::string cmd_result = Shell_exec::exec_get_ble_notification(_addr, 0);
	
	std::cout << cmd_result << std::endl;

	if(cmd_result == ""){
		throw myException(7, "get_message_from_device() : module " + _addr + " isn't running ", 1); 
	}

	std::string delimiter = "\n";
	std::string line;
	int pos;

	pos = cmd_result.find(delimiter);
	
	if(pos == std::string::npos){
		throw myException(8, "get_message_from_device() : parsing error ", 1);
	}

	line = cmd_result.substr(0, pos);
	cmd_result.erase(0, pos + delimiter.length());

	if(!parser.checkSubString(MSG_CHAR_VALUE_WRITTEN, line)){
		throw myException(8, "get_message_from_device() : parsing error ", 1);
	}

	std::string frame = ""; 

	while ((pos = cmd_result.find(delimiter)) != std::string::npos){
		line = cmd_result.substr(0, pos-1);
		cmd_result.erase(0, pos + delimiter.length());
		frame += parser.parse_notification(line);
	}

	return frame;
}

void BLE_manager::send_message_to_device(const std::string& _addr, std::string _message){
	
}