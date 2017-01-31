#include <iostream>
#include <exception>
#include <algorithm>
#include "GPS.h"


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
