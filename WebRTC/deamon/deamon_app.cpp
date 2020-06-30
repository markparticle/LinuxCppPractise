/**
 * server class
 * 
 * @author mark
 * @date 2020-5-16
 * @copyleft GPL 2.0
 * */

#include <iostream>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    //第一个参数切换到根目录
    //第二个参数将标准输入、输出、错误流重定向到哑设备
    if(daemon(0, 0) == -1){
        std::cout << "error\n" << std::endl;
        exit(-1);
    }
    while(true){
        sleep(1);
    }

    return 0;
}
