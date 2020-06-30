#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

const int PORT = 9999;
const int MESSAGE_LEN = 1024;
int main(){

    struct sockaddr_in addr, cline_addr;
    addr.sin_family = AF_INET;//ipv4c
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);//本地所有网卡的ip地址

    char buff_recv[MESSAGE_LEN];
    char buff_send[MESSAGE_LEN];
    int n;
    int cline_addr_len = sizeof(cline_addr);
    int sock_fd;
    //udp报文
    if((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        printf("socket error\n");
        exit(1);
    }

    if(bind(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0){
        printf("bind error\n");
        exit(1);
    } 
    printf("UDP Server start success...\n");
    while(true){
        n = recvfrom(sock_fd, buff_recv, MESSAGE_LEN - 1, 0, 
                (struct sockaddr*)&cline_addr, (socklen_t*)&cline_addr_len);
        std::cout << n << std::endl;
        if(n > 0){
            buff_recv[n] = '\0';
            printf("recv data from cline：%s %u says: %s\n",
                    inet_ntoa(cline_addr.sin_addr), ntohs(cline_addr.sin_port), buff_recv);
            printf("send message: ");
            scanf("%s", buff_send);
            n = sendto(sock_fd, buff_send, strlen(buff_send), 0, (struct sockaddr*)&cline_addr, cline_addr_len);
            if(n < 0){
                printf("sendto error.\n");
                break;
            }
        }
    }
    printf("UDP Server quite.\n");
    close(sock_fd);
    return 0;
}