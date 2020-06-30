/**
 * server class
 * 
 * @author mark
 * @date 2020-5-16
 * @copyleft GPL 2.0
 * */
#include <iostream>
#include <unistd.h>

#include "server.h"

namespace rtc{

Server::Server(){
    std::cout << "construct..." << std::endl;
}

Server::~Server(){
    std::cout << "destruct..." << std::endl;
}

void Server::run(){
    while(true){
        std::cout << "runing..." <<std::endl;
        ::usleep(1000000); //sleep one second
    }
}

} //namespace