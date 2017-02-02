#include <iostream>
#include "ble_manager.h"
#include "GPS.h"


#define GPS

#ifdef GPS

int main(void){

	GPS gps("/dev/ttyAMA0");
	
	gps.init();

	gps.start();

	sleep(10);

/*
	std::string str = "$G\nPGSV,1,1\n,00*79\n$GPRMC,0\n00336.80\n0,V,,,,,\n0.00,0.0\n0,060180\n,,,N*4C";

	std::replace(str.begin(), str.end(), "\n", "");

	std::cout << "str : " << str << std::endl;
	gps.process(str);
	std::cout << "str : " << str << std::endl;
*/
	gps.stop();
	std::cout << "End of program" << std::endl;
	return 0;
}
#endif

#ifdef BLE

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

#endif

