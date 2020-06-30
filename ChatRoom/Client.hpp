/*
 * @Author       : mark
 * @Date         : 2020-05-21
 * @copyleft GPL 2.0
 */ 

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "Common.hpp"

const char EXIT[] = "exit";

class Client {
public:
    Client();

    ~Client();

    void Start();

    void Init();

    void Close();
    
private:
    struct sockaddr_in serverAddr_;

    int socketFd_;

    int epollFd_;

    int pid_;

    int pipeFd_[2];

    bool isClientWork;

    char message_[BUFF_SIZE];
};

#endif //CLIENT_HPP