/**
 * server class
 * 
 * @author mark
 * @date 2020-5-16
 * @copyleft GPL 2.0
 * */

#include <iostream>
#include "server.h"

int main(int argc, char* argv[]){
    rtc::Server* server = new rtc::Server();
    if(server){
        server->run();
    }
    return 0;
}