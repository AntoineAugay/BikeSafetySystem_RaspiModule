


#include "ble_manager.h"

int main(int argc, char const *argv[])
{
	BLE_manager ble;
	std::string addr_rear;
	std::string addr_command;

	try {
		ble.initialization();
		ble.getModuleAddr(addr_rear, addr_command);
	} catch(myException& e) {
		std::cout << "ERROR " << e.getId() << " : " << e.what() << std::endl;
		
		if(e.getLevel() == 0){
			return e.getId();	
		}
	}

	std::cout << "REAR : " << addr_rear << std::endl;
	std::cout << "COMMAND : " << addr_command << std::endl;

	return 0;
}