#include <cstdlib>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <unistd.h> //read
using namespace std;

int WriteToTempFile(char * buffer, size_t length){
    char temp_filename[] = "/tmp/temp_file.XXXXXX";
    //mkstemp函数只有一个参数
    //参数是个以“XXXXXX”结尾的非空字符串
    //mkstemp函数会用随机产生的字符串替换“XXXXXX”，保证了文件名的唯一性
    int fd = mkstemp(temp_filename);

    unlink(temp_filename);
    //取消文件链接，不显示该临时文件

    /*首先写入即将写入数据的长度*/
    //write(fd, &length, sizeof(length));

    /*写入数据本身*/
    write(fd, buffer, length);
    return fd;
}

char *ReadFromTempFile(int fd, size_t length){
     /*把文件指针指向文件开头*/
    lseek(fd, 0, SEEK_SET);
    /*获得临时文件正文长度*/

    //read(fd, length, sizeof(*length));
     /*分配内存块，读取数据*/

    char *buffer = new char[length];
    read(fd, buffer, length);
    close(fd);
    //关闭文件描述符将删除该文件(未链接的文件)
    return buffer;
}

int main(){
    char buff[10] = "TEST!\n";
    int fd = WriteToTempFile(buff, sizeof(buff));
    printf("%s",  ReadFromTempFile(fd, sizeof(buff)));
}