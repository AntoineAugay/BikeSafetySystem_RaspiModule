#include <iostream>
#include "Application.h"


int main(void){

	Application app("/dev/ttyAMA0");
	app.start();
}

