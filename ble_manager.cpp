

#include "ble_manager.h"

BLE_manager::BLE_manager(){
}

BLE_manager::~BLE_manager(){
}

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
		throw myException(1, "No dongle BLE detected", 0);
	}

}

void BLE_manager::getModuleAddr(std::string& _addr_rear_module,std::string& _addr_command_module){
	
	std::string line = "";
	std::string delimiter = "\n";
	size_t pos = 0;

	/* Execute scan Shell command and collect result */
	std::string result = Shell_exec::exec_scan_ble();
	
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
		throw myException(2, "Modules not detected", 0);
	} else if (_addr_rear_module == "") {
		throw myException(3, "Rear module not detected", 1);
	} else if (_addr_command_module == "") {
		throw myException(4, "Command module not detected", 1);
	}
} 