#include "ble_manager.h"


int main(int argc, char const *argv[])
{
	BLE_manager ble;
	std::string addr_rear;
	std::string addr_command;

	std::string result("");
	
	try {
		ble.initialization();
		ble.get_module_addr(addr_rear, addr_command);
		
	} catch(myException& e) {
		std::cout << "ERROR " << e.getId() << " : " << e.what() << std::endl;
		
		if(e.getLevel() == 0){
			return e.getId();	
		}
	}

	try {
		result = ble.get_message_from_device(addr_command);
	} catch(myException& e) {
		std::cout << "ERROR " << e.getId() << " : " << e.what() << std::endl;
		
		if(e.getLevel() == 0){
			return e.getId();	
		}
	}
	
	std::cout << "message : " << result << std::endl;

	return 0;
}

