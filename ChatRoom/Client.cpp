/*
 * @Author       : mark
 * @Date         : 2020-05-22
 * @copyleft GPL 2.0
 */

#include <iostream>
#include "Client.hpp"

using namespace std;

Client::Client()
{
    serverAddr_.sin_family = AF_INET;
    serverAddr_.sin_port = htons(SERVER_PORT);
    serverAddr_.sin_addr.s_addr = inet_addr(SERVER_IP);
    socketFd_ = 0;
    epollFd_ = 0;
    pid_ = -1;
    isClientWork = true;
}

Client::~Client()
{
    Close();
}

void Client::Init()
{
    int ret;
    socketFd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd_ < 0)
    {
        perror("Create socket error!...");
        exit(-1);
    }
    cout << "Connect Server: " << SERVER_IP << " : " << SERVER_PORT << endl;
    ret = connect(socketFd_, (struct sockaddr *)&serverAddr_, sizeof(serverAddr_));
    if (ret < 0)
    {
        perror("Failed to connect server!...");
        exit(-1);
    }
    cout << "Connect Server success!..." << endl;

    if (pipe(pipeFd_) < 0)
    {
        perror("Pipe error!...");
        exit(-1);
    }
    epollFd_ = epoll_create(EPOLL_SIZE);
    if (epollFd_ < 0)
    {
        perror("Epoll error!...");
        exit(-1);
    }
    AddSocketFdToEpoll(socketFd_, epollFd_, true);
    //管道读端
    AddSocketFdToEpoll(pipeFd_[0], epollFd_, true);
}

void Client::Start()
{
    int ret;
    static struct epoll_event events[2];
    Init();
    pid_ = fork();
    if (pid_ < 0)
    {
        perror("Fork error!...");
        close(socketFd_);
        exit(-1);
    }
    else if (pid_ == 0)
    {
        //子进程 写入管道，关闭读入端
        cout << "[Server]Input 'exit' could exit the CharRoom." << endl;
        close(pipeFd_[0]);
        while (isClientWork)
        {
            bzero(&message_, BUFF_SIZE);
            fgets(message_, BUFF_SIZE, stdin);
            if (strncasecmp(message_, EXIT, strlen(EXIT)) == 0)
            {
                isClientWork = false;
            }
            else
            {
                ret = write(pipeFd_[1], message_, strlen(message_) - 1);
                if (ret < 0)
                {
                    perror("Write message error!...");
                    exit(-1);
                }
            }
        }
    }
    else
    {
        //pid > 0
        //父进程：读取管道数据发送数据，关闭写入端
        close(pipeFd_[1]);
        while (isClientWork)
        {
            int eventsCnt = epoll_wait(epollFd_, events, 2, TIMEOUT);

            for (int i = 0; i < eventsCnt; i++)
            {
                bzero(&message_, BUFF_SIZE);
                if (events[i].data.fd == socketFd_)
                {
                    ret = recv(socketFd_, message_, BUFF_SIZE, 0);
                    if (ret == 0)
                    {
                        cout << "Server closed!..." << endl;
                        isClientWork = false;
                        Close();
                    }
                    else
                    {
                        cout << message_ << endl;
                    }
                }
                else
                {
                    ret = read(events[i].data.fd, message_, BUFF_SIZE);
                    if (ret == 0)
                    {
                        isClientWork = false;
                    }
                    else
                    {
                        send(socketFd_, message_, BUFF_SIZE, 0);
                    }
                }
            }
        }
    }
    Close();
}

void Client::Close()
{
    if (pid_ > 0)
    {
        //父进程
        close(pipeFd_[0]);
        close(socketFd_);
        isClientWork = false;
        cout << "Exit the CharRoom!..." << endl;
        do
        {
            //等待所有子进程, 没有子进程会返回-1
            pid_ = waitpid(-1, NULL, 0);
        } while (pid_ != -1);
    }
    else
    {
        close(pipeFd_[1]);
    }
}

int main()
{
    Client client;
    client.Start();
    return 0;
}