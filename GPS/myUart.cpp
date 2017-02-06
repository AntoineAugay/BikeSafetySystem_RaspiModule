
#include "myUart.h"

MyUart::MyUart(std::string _uart) : uart(_uart), handle(-1) 
{ }


int MyUart::initUART(int _baudRate){

//-------------------------
	//----- SETUP USART 0 -----
	//-------------------------
	//At bootup, pins 8 and 10 are already set to UART0_TXD, UART0_RXD (ie the alt0 function) respectively

	//OPEN THE UART
	//The flags (defined in fcntl.h):
	//	Access modes (use 1 of these):
	//		O_RDONLY - Open for reading only.
	//		O_RDWR - Open for reading and writing.
	//		O_WRONLY - Open for writing only.
	//
	//	O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
	//											if there is no input immediately available (instead of blocking). Likewise, write requests can also return
	//											immediately with a failure status if the output can't be written immediately.
	//
	//	O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.
	handle = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (handle == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
		return -1;
	}

	//CONFIGURE THE UART
	//The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)
	struct termios options;
	tcgetattr(handle, &options);

	if(_baudRate == 9600){
		options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;		//<Set baud rate	
	} else if (_baudRate == 57600) {
		options.c_cflag = B57600 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	} else if (_baudRate == 115200) {
		options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	} else {
		return -2;
	}
	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(handle, TCIFLUSH);
	tcsetattr(handle, TCSANOW, &options);
	return handle;
}

	
int MyUart::readUART(char buffer[256]){

	if (handle == -1) {
		return -1;
	} else {
		// Read up to 255 characters from the port if they are there
		int length = read(handle, (void*)buffer, 255);		//Filestream, buffer to store in, number of bytes to read (max)
		if (length < 0)
		{
			//An error occured (will occur if there are no bytes)
			return -1;
		}
		else if (length == 0)
		{
			//No data waiting
			return 0;
		}
		else
		{
			//Bytes received
			buffer[length] = '\0';
			//printf("%i bytes read : %s\n", length, buffer);
			return length;
		}
	}
}


int MyUart::writeUART(const char * frame) {
	//----- TX BYTES -----
	if (handle == -1) {
		return -1;
	} else {
		
		int count = write(handle, frame, sizeof(frame));		//Filestream, bytes to write, number of bytes to write
		
		if (count < 0)
		{
			printf("UART TX error\n");
		}
	}
}

void MyUart::closeUART(void){
	close(handle);
}