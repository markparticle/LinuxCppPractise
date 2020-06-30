#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

const int PORT = 8111;
const int MESSAGE_LEN = 1024;

int main(int argc, char* argv[]){

    int on = 1;
    int ret;
    int backlog = 10;
    char in_buff[MESSAGE_LEN] = {0};
    pid_t pid;

    int socket_fd, accept_fd;
    struct sockaddr_in localaddr, remoteaddr;


    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd == -1){
        std::cout << "Failed to create socket!" << std::endl;
        exit(-1);
    }

    ret = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)); 
    if(ret == -1) {
        std::cout << "Failed to set socket options!" << std::endl;
        exit(-1);
    }

    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(PORT);
    localaddr.sin_addr.s_addr = INADDR_ANY;//0
    bzero(&(localaddr.sin_zero), 8);//8个字节全置为0

    ret = bind(socket_fd, (struct sockaddr*)&localaddr, sizeof(struct sockaddr));
    if(ret == - 1){
        std::cout << "Failed to bind addr!" << std::endl;
        exit(-1);
    }
    
    ret = listen(socket_fd, backlog);
    if(ret ==-1){
        std::cout << "Failed to listen socket!" << std::endl;
        exit(-1);
    }
    printf("server start...\n");
    while(true){
        socklen_t addr_len =  sizeof(struct sockaddr);
        accept_fd = accept(socket_fd, (struct sockaddr*) &remoteaddr, &addr_len);
        pid = fork();
        //子进程
        if(pid == 0){
            while(true){
                memset(in_buff, 0, MESSAGE_LEN);
                ret = recv(accept_fd, (void *)in_buff, MESSAGE_LEN, 0);
                if(ret == 0){
                    break;
                }
                std::cout << "recv: " << in_buff <<std::endl;
                send(accept_fd, (void*)in_buff, MESSAGE_LEN, 0);
            }
            printf("close client connection...\n");
            close(accept_fd);
        }
    }
    if(pid != 0){
        printf("quit server...\n");
        close(socket_fd);
    }
    return 0;
}