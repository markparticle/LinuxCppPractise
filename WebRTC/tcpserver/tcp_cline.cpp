#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

const int PORT = 8111;
const int MESSAGE_LEN = 1024;

int main(int argc, char* argv[]){

    int socket_fd;
    int ret;
    char sendbuf[MESSAGE_LEN] = {0};
    char recvbuf[MESSAGE_LEN] = {0};

    struct sockaddr_in serveraddr;


    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0){
        std::cout << "Failed to create socket!" << std::endl;
        exit(-1);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");//inet_addr将地址转为网络字节序ip

    ret = connect(socket_fd, (struct sockaddr*)&serveraddr, sizeof(struct sockaddr));
    if(ret < 0){
        std::cout << "Failed to connect server!" << std::endl;
        exit(-1);
    }
    printf("connect server success!\n");
    while(true){
        memset(sendbuf, 0, MESSAGE_LEN);
        printf(">>> send message: ");
        fgets(sendbuf, MESSAGE_LEN, stdin);
        ret = send(socket_fd, sendbuf, strlen(sendbuf), 0);
        if(ret <= 0){
            std::cout << "Failed to send data!" << std::endl;
            break;
        }
        if(strcmp(sendbuf, "quit\n") == 0){
            break;
        }
        printf(">>> echo message: ");
        recvbuf[0] = '\0';
        ret = recv(socket_fd, recvbuf, MESSAGE_LEN, 0);
        if(ret <= 0){
            std::cout << "Failed to rec data!" << std::endl;
            break;
        }
        recvbuf[ret] = '\0';
        std::cout << recvbuf;
    }
    std::cout << "Cline quit!" << std::endl;
    close(socket_fd);

    return 0;
}