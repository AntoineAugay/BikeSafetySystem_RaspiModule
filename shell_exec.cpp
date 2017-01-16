
#include "shell_exec.h"




std::string Shell_exec::exec(std::string _cmd){
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(_cmd.c_str(), "r");
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


std::string Shell_exec::exec_scan_ble(){

    fd_set set;
    struct timeval timeout = {1,0}; //timeout of 10 secs.
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
    
    system("sudo hciconfig hci0 down");
    system("sudo hciconfig hci0 up");
    return result;

}
	 