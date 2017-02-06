
#include "shell_exec.h"




std::string ShellExec::exec(std::string cmd){
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}


std::string ShellExec::execScan(){

    fd_set set;
    struct timeval timeout = {0,0}; //timeout of 10 secs.
    int ret = -1;

    char buffer[128];
    std::string result = "";
    FILE* pipe = popen("sudo stdbuf -oL hcitool lescan", "r");
    int fd = fileno(pipe);

    FD_ZERO(&set);
    FD_SET(fd, &set);

    if (!pipe) throw std::runtime_error("popen() failed!");
    try {

        while ( (!feof(pipe)) && (ret != 0) ) {

            timeout = {1,0};
            ret = select(FD_SETSIZE, &set, NULL, NULL, &timeout);

            if( ret == (-1) ){

            } else if( ret ) {

                if ( fgets(buffer, 128, pipe) != NULL ){
                    result += buffer;
                }
            }
        }

        system("sudo pkill hcitool");
   
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    
    system("sudo hciconfig hci1 down");
    system("sudo hciconfig hci1 up");
    return result;
}
	
std::string ShellExec::execGetBleNotification(std::string addr, int timeout_ms){

    fd_set set;

    int sec = timeout_ms/1000;
    int usec = (timeout_ms - sec*1000)*1000;

    struct timeval timeout = {sec,usec}; //timeout of 10 secs.
    int ret = -1;

    char buffer[128];
    std::string result = "";
    std::string cmd = "sudo gatttool -t random -b " + addr + " --char-write-req --handle=0x000e --value=0100 --listen";
    FILE* pipe = popen(cmd.c_str(), "r");
    int fd = fileno(pipe);

    FD_ZERO(&set);
    FD_SET(fd, &set);

    
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while ( (!feof(pipe)) && (ret != 0) ) {
            timeout = {1,0};
            ret = select(FD_SETSIZE, &set, NULL, NULL, &timeout);

            if( ret == (-1) ){

            } else if( ret ) {

                if ( fgets(buffer, 128, pipe) != NULL ){
                    result += buffer;
                }
            }
        }

        system("sudo pkill gatttool");
   
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    
    return result;
}
    