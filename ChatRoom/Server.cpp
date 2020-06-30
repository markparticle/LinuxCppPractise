/*
 * @Author       : mark
 * @Date         : 2020-05-22
 * @copyleft GPL 2.0
 */
#include <iostream>
#include "Server.hpp"

using namespace std;

Server::Server()
{
    serverAddr_.sin_family = AF_INET;
    serverAddr_.sin_port = htons(SERVER_PORT);
    serverAddr_.sin_addr.s_addr = inet_addr(SERVER_IP);
    socketFd_ = 0;
    epollFd_ = 0;
}

Server::~Server()
{
    clientsList_.clear();
    Close();
}

void Server::Init()
{
    int ret;
    cout << "Start Server..." << endl;
    socketFd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd_ < 0)
    {
        perror("Create socket error!...");
        exit(-1);
    }

    ret = bind(socketFd_, (struct sockaddr *)&serverAddr_, sizeof(serverAddr_));
    if (ret < 0)
    {
        perror("Bind socket error!...");
        exit(-1);
    }

    ret = listen(socketFd_, 6);
    if (ret < 0)
    {
        perror("Listen socket error!...");
        exit(-1);
    }
    cout << "Start to listen: " << SERVER_IP << ":" << SERVER_PORT << endl;

    //内核中创建事件
    epollFd_ = epoll_create(EPOLL_SIZE);
    if (epollFd_ < 0)
    {
        perror("Create epoll error!...");
        exit(-1);
    }
    //添加监听事件
    AddSocketFdToEpoll(socketFd_, epollFd_, true);
}

void Server::Close()
{
    close(socketFd_);
    close(epollFd_);
    cout << "Server Closed!..." << endl;
}

void Server::CloseClientFd(const int clientFd)
{
    close(clientFd);
    clientsList_.remove(clientFd);

    cout << "Client:" << clientFd
         << " quited!...\nNow there are "
         << clientsList_.size()
         << " client in the char room"
         << endl;
}

void Server::Start()
{
    static struct epoll_event events[EPOLL_SIZE];
    Init();
    int ret;
    while (true)
    {
        int eventsCnt = epoll_wait(epollFd_, events, EPOLL_SIZE, TIMEOUT);
        if (eventsCnt < 0)
        {
            perror("Epoll events error!...");
            break;
        }
        for (int i = 0; i < eventsCnt; i++)
        {
            int eventSockFd = events[i].data.fd;
            //建立一个新的连接
            if (eventSockFd == socketFd_)
            {
                struct sockaddr_in clientAddr;
                socklen_t addrLen = sizeof(struct sockaddr);
                int clientFd = accept(socketFd_, (struct sockaddr *)&clientAddr, &addrLen);

                cout << "Client from: "
                     << inet_ntoa(clientAddr.sin_addr) << " : "
                     << ntohs(clientAddr.sin_port) << ", clientFd: "
                     << clientFd << endl;

                AddSocketFdToEpoll(clientFd, epollFd_, true);
                clientsList_.push_back(clientFd);
                cout << "Add newClinent : " << clientFd << " to epoll..." << endl;
                cout << "Now there are " << clientsList_.size() << " clients in the ChatRoom..." << endl;

                char message[BUFF_SIZE];
                bzero(message, BUFF_SIZE);
                sprintf(message, SERVER_WELCOME, clientFd);
                ret = send(clientFd, message, BUFF_SIZE, 0);
                if (ret < 0)
                {
                    perror("Send message to error!...");
                    CloseClientFd(clientFd);
                    exit(-1);
                }
                bzero(message, BUFF_SIZE);
                sprintf(message, CLIENT_JOIN, clientFd);
                SendToAllClient(message, clientFd);
            }
            else
            {
                ret = SendBroadCastMessage(eventSockFd);
            }
        }
    }
    Close();
}

int Server::SendToAllClient(const char *message, const int clientFd)
{
    int ret = -1;
    for (auto &item : clientsList_)
    {
        if (item == clientFd)
        {
            continue;
        }
        ret = send(item, message, BUFF_SIZE, 0);
        if (ret < 0)
        {
            perror("Send broadCast message error!...");
            CloseClientFd(item);
        }
    }
    return ret;
}

int Server::SendBroadCastMessage(const int clientFd)
{
    char buff[BUFF_SIZE], message[BUFF_SIZE];
    bzero(buff, BUFF_SIZE);
    bzero(message, BUFF_SIZE);
    int ret;

    int len = recv(clientFd, buff, BUFF_SIZE, 0);
    if (len == 0)
    {
        CloseClientFd(clientFd);
    }
    else
    {
        if (clientsList_.size() == 1)
        {
            // 发送提示消息
            send(clientFd, CAUTION, strlen(CAUTION), 0);
            return len;
        }

        sprintf(message, SERVER_MESSAGE, clientFd, buff);
        cout << message << endl;
        SendToAllClient(message, clientFd);
    }
    return len;
}

int main()
{
    Server server;
    server.Start();
}