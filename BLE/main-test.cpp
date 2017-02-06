#include <iostream>
#include <exception>
#include <sys/time.h>
#include <unistd.h>
#include "ble_manager.h"


int main(int argc, char const *argv[])
{
	BLEManager ble;
	int err = 0;
	int n = 100;

	std::string frame("");

	try {
		err = ble.init();

		if(err < 0){
			std::cout << "init ERROR : " << err << std::endl;
			return -1;
		}


		ble.start();

		sleep(60);

		ble.stop();
	} catch(std::exception const& e) {
		std::cout << "ERROR : " << e.what() << std::endl;
	}


	/*
	try {
		
		err = ble.init();
		if(err < 0){
			std::cout << "init ERROR : " << err << std::endl;
			return -1;
		}
		

		gettimeofday(&tvStart, 0);
		long start = (tvStart.tv_sec*1000) + (tvStart.tv_usec/1000);

		err = ble.getModuleAddr();
		if(err < 0){
			std::cout << "scan ERROR : " << err << std::endl;
			if(err == -3)
				return -1;
		}

		gettimeofday(&tvStop, 0);
		long stop = (tvStop.tv_sec*1000) + (tvStop.tv_usec/1000);
		std::cout << "Time (scan) : " << stop-start << std::endl;

		if(err != -1){
			while (n-- != 0) {
				
				gettimeofday(&tvStart, 0);
				long start = (tvStart.tv_sec*1000) + (tvStart.tv_usec/1000);

				err = ble.getMessageFromDevice(COMMAND, frame);

				gettimeofday(&tvStop, 0);
				long stop = (tvStop.tv_sec*1000) + (tvStop.tv_usec/1000);
				std::cout << "Time (receive) : " << stop-start << std::endl;

				if(err < 0){
					std::cout << " getmsg ERROR : " << err << std::endl;
					if(err == -1 || err == -2){
						return -1;
					} else {
						continue;
					}
					
				}

				if(frame != ""){
					ble.parseCommandFrame(frame);
				}
			}
		}
		
		
	} catch(std::exception const& e) {
		std::cout << "ERROR : " << e.what() << std::endl;
	}
	*/
	return 0;
}
