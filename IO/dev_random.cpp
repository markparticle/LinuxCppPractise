#include <fcntl.h>   //open
#include <unistd.h>  //read
#include <iostream>
using namespace std;


int get_random(int L, int R){
    int fd = -1;
    unsigned random_val;
    char* random_buff;
    int buff_length;

    random_buff =  (char *)&random_val;
    buff_length = sizeof(random_val);

    //urandom不会阻塞
    //random通过输入活动的延迟来作为随机数
    fd = open("/dev/urandom", O_RDONLY);
    random_val = read(fd, random_buff, buff_length);

    do{
        int nfd =  read(fd, random_buff, buff_length);
        buff_length -= nfd;
        random_buff += nfd;
    } while(buff_length > 0);
    close(fd);
    return L + random_val % (R - L + 1);
}

int main(){
    int n = 100;
    for(int i = 0; i < n; i++){
        printf("%d\n", get_random(1, 10));
    }
    return 0;
}
