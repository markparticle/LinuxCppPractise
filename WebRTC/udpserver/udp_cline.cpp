#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

const int PORT = 9999;
const int MESSAGE_LEN = 1024;

int main(){
    int sock_fd;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    char buff_send[MESSAGE_LEN] = "HELLO!";
    char buff_recv[MESSAGE_LEN] = {0};
    int addr_len = sizeof(addr);

    int n = 0;

    if((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        printf("socket error.\n");
        exit(1);
    }
    if(addr.sin_addr.s_addr == INADDR_NONE){
        printf("Incorrect IP address!\n");
        close(sock_fd);
        exit(1);
    }
    printf("send message:");
    scanf("%s", buff_send);
    n = sendto(sock_fd, buff_send, strlen(buff_send), 0, (struct sockaddr*)&addr, sizeof(addr));
    if(n < 0){
        printf("sendto error.\n");
        close(sock_fd);
    }
    n = recvfrom(sock_fd, buff_recv, MESSAGE_LEN, 0, (struct sockaddr*)&addr, (socklen_t*)&addr_len);
    if(n > 0){
        buff_recv[n] = '\0';
        printf("received from server: %s\n", buff_recv);
    }
    else if(n == 0){
        printf("server close.\n");
    } 
    else if(n == -1){
        printf("recvfrom error.\n");
    }
    printf("UDP cline quite.\n");
    close(sock_fd);
    return 0;
}