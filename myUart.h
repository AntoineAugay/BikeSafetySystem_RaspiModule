#ifndef MYUART_H
#define MYUART_H


#include <string>
#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART

class MyUart 
{
private : 
	
	std::string uart;
	int handle;
	
public :

	MyUart(std::string _uart);
	
	int initUART(int _baudRate);
	
	int readUART(char* buffer);

	int writeUART(const char* frame);

	void closeUART(void);

};

#endif