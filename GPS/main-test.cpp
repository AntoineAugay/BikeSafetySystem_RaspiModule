#include <iostream>
#include "GPS.h"

int main(void){

	int n = 10;
	int err = 0;
	GPSInfo gpsInfo;

	GPS gps("/dev/ttyAMA0");
	gps.init();
	gps.start();

	while (n-- > 0){
		err = gps.getGPSInfo(gpsInfo);
		if(err < 0){
			std::cout << "Parse succesful" << std::endl;
		} else {
			std::cout << "Parse fail" << std::endl;
		}
		sleep(1);
	}


	gps.stop();
	std::cout << "End of program" << std::endl;
	return 0;
}


