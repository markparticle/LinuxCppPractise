#include <iostream>
#include <string.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

const int PORT = 8111;
const int MESSAGE_LEN = 1024;
const int MAX_EVENTS = 20; //同一时刻的事件并发数
const int TIMEOUT = 500;
const int MAX_PROCESS = 4 * 2 + 1; //cpu核心数*2 + 1

int main(int argc, char *argv[])
{

    int on = 1;
    int ret;
    int backlog = 10;
    char in_buff[MESSAGE_LEN] = {0};

    int socket_fd, accept_fd;
    struct sockaddr_in localaddr, remoteaddr;

    int flags;
    int epoll_fd;
    int event_number;
    struct epoll_event ev, events[MAX_EVENTS];

    pid_t pid = -1;

    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(PORT);
    localaddr.sin_addr.s_addr = INADDR_ANY; //0
    bzero(&(localaddr.sin_zero), 8);        //8个字节全置为0

    //建立socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        std::cout << "Failed to create socket!" << std::endl;
        exit(-1);
    }

    //设置为非阻塞
    flags = fcntl(socket_fd, F_GETFL, 0);
    fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);

    //设置参数
    ret = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (ret == -1)
    {
        std::cout << "Failed to set socket options!" << std::endl;
        exit(-1);
    }

    //绑定socket
    ret = bind(socket_fd, (struct sockaddr *)&localaddr, sizeof(struct sockaddr));
    if (ret == -1)
    {
        std::cout << "Failed to bind addr!" << std::endl;
        exit(-1);
    }

    //监听
    ret = listen(socket_fd, backlog);
    if (ret == -1)
    {
        std::cout << "Failed to listen socket!" << std::endl;
        exit(-1);
    }

    //多进程
    for (int i = 1; i < MAX_PROCESS; i++)
    {
        if (pid != 0)
        {
            pid = fork();
        }
    }

    //子进程
    if (pid == 0)
    {
        printf("%d server start...\n", getpid());
        epoll_fd = epoll_create(256);
        ev.events = EPOLLIN;
        ev.data.fd = socket_fd;
        //添加一个文件描述符
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &ev);

        while (true)
        {
            event_number = epoll_wait(epoll_fd, events, MAX_EVENTS, TIMEOUT);

            for (int i = 0; i < event_number; i++)
            {
                if (events[i].data.fd == socket_fd)
                {
                    //接收一个新的连接
                    printf("%d listen event...\n", getpid());
                    socklen_t addr_len = sizeof(struct sockaddr);
                    accept_fd = accept(socket_fd, (struct sockaddr *)&remoteaddr, &addr_len);
                    //设置为非阻塞
                    flags = fcntl(accept_fd, F_GETFL, 0);
                    fcntl(accept_fd, F_SETFL, flags | O_NONBLOCK);

                    //边缘触发 没有比连接的重要性更大
                    ev.events = EPOLLIN | EPOLLET;
                    ev.data.fd = accept_fd;
                    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, accept_fd, &ev);
                }
                else if (events[i].events & EPOLLIN)
                {
                    //输入事件
                    do
                    {
                        memset(in_buff, 0, MESSAGE_LEN);
                        ret = recv(events[i].data.fd, (void *)in_buff, MESSAGE_LEN, 0);
                        if (ret == 0)
                        {
                            printf("%d close client connection...\n", getpid());
                            close(events[i].data.fd);
                        }
                        else if (ret == MESSAGE_LEN)
                        {
                            printf("Maybe have data...\n");
                        }
                        //EINTR 操作被中断了
                    } while (ret < 0 && errno == EINTR);

                    if (ret < 0)
                    {
                        switch (errno)
                        {
                        //没有数据了
                        case EAGAIN:
                            break;
                        default:
                            break;
                        }
                    }
                    else if (ret > 0)
                    {
                        printf("%d receive message: %s", getpid(), in_buff);
                        send(events[i].data.fd, (void *)in_buff, MESSAGE_LEN, 0);
                    }
                }
            }
        }
        printf("quit server...\n");
        close(socket_fd);
    }
    else
    {
        //等待子进程结束
        do
        {
            //等待所有子进程, 没有子进程会返回-1
            pid = waitpid(-1, NULL, 0);
        } while (pid != -1);
    }
    return 0;
}