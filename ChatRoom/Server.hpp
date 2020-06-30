/*
 * @Author       : mark
 * @Date         : 2020-05-21
 * @copyleft GPL 2.0
 */ 

#ifndef SERVER_H
#define SERVER_H
#include "Common.hpp"

const char SERVER_WELCOME[] = "[Server]Welcome you join to the CharRoom!\n[Server]Your chat ID is: %d";
const char CLIENT_JOIN[] = "[Server]Welcome %d join to the CharRoom!";
const char SERVER_MESSAGE[] = "[%d]: %s";  
const char CAUTION[] =  "[Server]There is only one int the char room!";

class Server {
public:
    Server();

    ~Server();

    void Init();

    void Start();

    void Connect();

    void Close();

    void CloseClientFd(const int clientFd);

    int SendBroadCastMessage(const int clientFd);
    
    int SendToAllClient(const char *message, const int clientFd = -1);
    
private:
    int socketFd_;

    struct sockaddr_in serverAddr_;

    int epollFd_;

    std::list<int> clientsList_;
};


#endif //SERVER_H