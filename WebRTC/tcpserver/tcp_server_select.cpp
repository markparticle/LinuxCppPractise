#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

const int PORT = 8111;
const int MESSAGE_LEN = 1024;
const int FD_SIZE = 1024;

int main(int argc, char *argv[])
{

    int on = 1;
    int ret;
    int backlog = 10;
    char in_buff[MESSAGE_LEN] = {0};

    int flags;
    int events;
    int max_fd = -1;
    int max_pos = 0;
    int curpos = -1;
    fd_set fd_sets;
    int accept_fds[FD_SIZE] = {-1};

    int socket_fd, accept_fd;
    struct sockaddr_in localaddr, remoteaddr;

    //set local address
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(PORT);
    localaddr.sin_addr.s_addr = INADDR_ANY; //0
    bzero(&(localaddr.sin_zero), 8);        //8个字节全置为0

    //创建套接字
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        std::cout << "Failed to create socket!" << std::endl;
        exit(-1);
    }
    //设置套接字 SOL_SOCKET是被设置的选项的级别，SO_REUSEADDR，地址复用功能
    ret = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (ret == -1)
    {
        std::cout << "Failed to set socket options!" << std::endl;
        exit(-1);
    }
    //设置为非阻塞
    flags = fcntl(socket_fd, F_GETFL, 0);
    fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);

    //绑定socket
    ret = bind(socket_fd, (struct sockaddr *)&localaddr, sizeof(struct sockaddr));
    if (ret == -1)
    {
        std::cout << "Failed to bind addr!" << std::endl;
        exit(-1);
    }
    //监听socket
    ret = listen(socket_fd, backlog);
    if (ret == -1)
    {
        std::cout << "Failed to listen socket!" << std::endl;
        exit(-1);
    }
    printf("server start...\n");


    max_fd = socket_fd;
    memset(accept_fds, -1, sizeof(accept_fds));

    while (true)
    {
        FD_ZERO(&fd_sets); //清空fd_sets
        FD_SET(socket_fd, &fd_sets);  //将socket_fd添加到sets

        for (int i = 0; i < max_pos; i++)
        {
            if (accept_fds[i] != -1)
            {
                if (accept_fds[i] > max_fd)
                {
                    max_fd = accept_fds[i];
                }
                printf("fd_sets update：fd: %d, idx:%d, max_fd:%d\n", accept_fds[i], i, max_fd);
                FD_SET(accept_fds[i], &fd_sets);
            }
        }

        events = select(max_fd + 1, &fd_sets, NULL, NULL, NULL);
        if (events < 0)
        {
            printf("Failed to use select!\n");
            break;
        }
        else if (events == 0)
        {
            printf("timeout...\n");
        }
        else if (events > 0)
        {
            printf("events:%d\n", events);
            //遍历所有的fd
            if (FD_ISSET(socket_fd, &fd_sets))
            {
                //新的连接
                printf("listen event :\n");
                int i = 0;
                for (; i < FD_SIZE; i++)
                {
                    if (accept_fds[i] == -1)
                    {
                        curpos = i;
                        break;
                    }
                }
                if(i == FD_SIZE) {
                    printf("the connection is full!\n");
                    continue;
                }

                //创建一个新的连接
                socklen_t addr_len = sizeof(struct sockaddr);
                accept_fd = accept(socket_fd, (struct sockaddr *)&remoteaddr, &addr_len);
                //设置为非阻塞
                flags = fcntl(accept_fd, F_GETFL, 0);
                fcntl(accept_fd, F_SETFL, flags | O_NONBLOCK);

                accept_fds[curpos] = accept_fd;
                if(curpos + 1 > max_pos){
                    max_pos = curpos + 1;
                }
                if(accept_fd > max_fd){
                    max_fd = accept_fd;
                }
                printf("new connection fd:%d, curpos = %d \n", accept_fd, curpos);
            }
            //接收信息
            for (int i = 0; i < max_pos; i++)
            {   
                //有事件
                if (accept_fds[i] != -1 && FD_ISSET(accept_fds[i], &fd_sets))
                {
                    printf("accept event : %d, accept_fd: %d\n", i, accept_fds[i]);
                    memset(in_buff, 0, MESSAGE_LEN);
                    ret = recv(accept_fds[i], (void *)in_buff, MESSAGE_LEN, 0);
                    if (ret == 0)
                    {
                        printf("close client connection...\n");
                        close(accept_fds[i]);
                        accept_fds[i] = -1;
                    }
                    std::cout << "recv: " << in_buff << std::endl;
                    send(accept_fds[i], (void *)in_buff, MESSAGE_LEN, 0);
                }
            }
        }
    }

    printf("quit server...\n");
    close(socket_fd);

    return 0;
}